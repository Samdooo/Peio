#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 0.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f

#define numDims 3
#define maxBounces 4

struct VSOutput {
	float4 position : SV_POSITION;
	float ray[numDims] : VS_RAY;
};

struct Camera {
	float position[numDims];
	float rotation[numDims - 1];
	float fov;
};
StructuredBuffer<Camera> camera : register(t0);

struct Scene {
	uint2 windowSize;
	uint numRects;
	uint numSpheres;
	float3 backLight;
	uint numBounces;
};
StructuredBuffer<Scene> scene : register(t1);
StructuredBuffer<uint> raysPerBounce : register(t2);

struct Material {
	float3 emission;
	float3 reflection;
	float spectral;
};

struct HyperRect {
	float position[numDims];
	float size[numDims];
	Material material;
};
StructuredBuffer<HyperRect> hyperRects : register(t3);

struct HyperSphere {
	float center[numDims];
	float radius;
	Material material;
};
StructuredBuffer<HyperSphere> hyperSpheres : register(t4);

StructuredBuffer<uint> seeds : register(t5);
float randFloat(uint2 pixelPosition, uint add) {
    uint seed = seeds[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];
    return frac(cos((float)(seed + add) * PHI) * 12345.6789f);
}

struct Vector {
	float v[numDims];	
};
StructuredBuffer<Vector> vectors : register(t6);

struct Collision {
	float ray[numDims];
	float position[numDims];
	float normal[numDims];
	float scale;
	Material material;
};

float Dot(float first[numDims], float second[numDims]) {
	float dot = 0.0f;
	for (uint i = 0; i < numDims; i++)
		dot += first[i] * second[i];
	return dot;
}

void Normalize(out float vec[numDims]) {
	float length = 0.0f;
	for (uint i = 0; i < numDims; i++)
		length += vec[i];
	length = sqrt(length);
	for (uint i = 0; i < numDims; i++)
		vec[i] /= length;
}

Collision Trace(float origin[numDims], float ray[numDims]) {
	Collision col;
	col.ray = ray;
	
	float bestScale = 1.#INF;
	
	const uint numRects = scene[0].numRects;
	const float lim = 1.0f / (float)(1 << 16);
	
	[loop] for (uint i = 0; i < numRects; i++){
		HyperRect rect = hyperRects[i];
		
		float sMin = -1.#INF;
		float sMax = 1.#INF;
		
		for (uint j = 0; j < numDims; j++){
			float first = (rect.position[j] - origin[j]) / ray[j];
			float second = (rect.position[j] + rect.size[j] - origin[j]) / ray[j];
			if (min(first, second) > sMin)
				sMin = min(first, second);
			if (max(first, second) < sMax)
				sMax = max(first, second);
		}
		if (sMin > sMax || sMax <= lim)
			continue;
		float scale = sMin;
		if (scale <= lim)
			scale = sMax;
		
		if (scale < bestScale){
			bestScale = scale;
			col.material = rect.material;
			for (uint j = 0; j < numDims; j++){
				col.position[j] = origin[j] + ray[j] * scale;
				if (abs(col.position[j] - rect.position[j]) <= lim)
					col.normal[j] = -1.0f;
				else if (abs(col.position[j] - (rect.position[j] + rect.size[j])) <= lim)
					col.normal[j] = 1.0f;
				else
					col.normal[j] = 0.0f;
			}
		}
	}
	
	const uint numSpheres = scene[0].numSpheres;
	
	[loop] for (uint i = 0; i < numSpheres; i++){
		HyperSphere sphere = hyperSpheres[i];
		
		float a = 0.0f, b = 0.0f, c = 0.0f;
		for (uint j = 0; j < numDims; j++){
			a += ray[j] * ray[j];
			b += 2.0f * (origin[j] * ray[j] - sphere.center[j] * ray[j]);
			c += (origin[j] - sphere.center[j]) * (origin[j] - sphere.center[j]) - sphere.radius * sphere.radius;
		}
		if ((b * b - 4.0f * a * c) < 0.0f)
			continue;
		
		float first = -b / (2.0f * a);
		float second = abs(sqrt(b * b - 4.0f * a * c) / (2.0f * a));
		
		float sMin = first - second;
		float sMax = first + second;
		
		if (sMax <= lim)
			continue;
		float scale = sMin;
		if (scale <= lim)
			scale = sMax;
		if (scale < bestScale){
			bestScale = scale;
			col.material = sphere.material;
			for (uint j = 0; j < numDims; j++){
				col.position[j] = origin[j] + ray[j] * scale;
				col.normal[j] = col.position[j] - sphere.center[j];
			}
		}
	}
	col.scale = bestScale;
	Normalize(col.normal);
	return col;
}

float3 BackLight(float ray[numDims]){
	return scene[0].backLight;
}

void RandVector(uint2 pixelPosition, out float vec[numDims], uint add){
	[loop] while (true) {
		float dist = 0.0f;
		for (uint i = 0; i < numDims; i++){
			vec[i] = randFloat(pixelPosition, add);
			dist += vec[i] * vec[i];
			add++;
		}
		if (dist <= 1.0f){
			if (dist == 0.0f)
				vec[0] = 1.0f;
			break;
		}
	}
}

float3 Light(uint2 pixelPosition, float origin[numDims], float ray[numDims]){
	
	const uint numBounces = scene[0].numBounces;
	Collision cols[maxBounces];
	float3 muls[maxBounces];
	uint diffuseCount[maxBounces];
	
	diffuseCount[0] = 0;
	muls[0] = float3(1.0f, 1.0f, 1.0f);
	cols[0] = Trace(origin, ray);
	
	float3 totalLight = 0.0f;
	uint curBounce = 0;
	bool up = true;
	
	uint vectorIndex = seeds[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];
	vectorIndex %= (1 << 16);
	
	uint numIts = 0;
	[loop] while (true){
		numIts++;
		if (up){
			if (cols[curBounce].scale == 1.#INF){
				up = false;
				totalLight += muls[curBounce] * BackLight(cols[curBounce].ray);
			}
			else {
				totalLight += muls[curBounce] * cols[curBounce].material.emission;
				if (curBounce == numBounces - 1){
					up = false;
				}
				else {
					up = true;
					muls[curBounce + 1] = muls[curBounce] * cols[curBounce].material.reflection * cols[curBounce].material.spectral;
					float spectralRay[numDims];
					float dot = Dot(cols[curBounce].ray, cols[curBounce].normal);
					for (uint i = 0; i < numDims; i++)
						spectralRay[i] = cols[curBounce].ray[i] - cols[curBounce].normal[i] * 2.0f * dot;
					cols[curBounce + 1] = Trace(cols[curBounce].position, spectralRay);
				}
			}
		}
		else {
			if (diffuseCount[curBounce] == raysPerBounce[curBounce] - 1){
				up = false;
			}
			else {
				up = true;
				diffuseCount[curBounce]++;
				
				muls[curBounce + 1] = muls[curBounce] * cols[curBounce].material.reflection * (1.0f - cols[curBounce].material.spectral) * (1.0f / (float)(raysPerBounce[curBounce] - 1));
				
				float diffuseRay[numDims];
				//RandVector(pixelPosition, diffuseRay, randAdd);
				diffuseRay = vectors[vectorIndex];
				vectorIndex++;
				vectorIndex %= (1 << 16);
				
				float dot = Dot(diffuseRay, cols[curBounce].normal);
				if (dot < 0.0f){
					for (uint i = 0; i < numDims; i++)
						diffuseRay[i] = -diffuseRay[i];
				}
				cols[curBounce + 1] = Trace(cols[curBounce].position, diffuseRay);
			}
		}
		if (up){
			curBounce++;
			diffuseCount[curBounce] = 0;
		}
		else {
			if (curBounce == 0)
				break;
			curBounce--;
		}
	}
	//totalLight = float3((float)numIts / 255.0f, (float)numIts / 255.0f,(float)numIts / 255.0f);
	return totalLight;
}

float4 main(VSOutput input) : SV_TARGET
{
	float position[numDims] = camera[0].position;
	Normalize(input.ray);
	float ray[numDims] = input.ray;

	//return float4(Light((uint2)input.position.xy, position, ray), 1.0f);
	Collision col = Trace(position, ray);
	if (col.scale == 1.#INF)
		return float4(BackLight(ray), 1.0f);

	float spectralRay[numDims];
	float dot = Dot(col.ray, col.normal);
	for (uint i = 0; i < numDims; i++)
		spectralRay[i] = col.ray[i] - col.normal[i] * 2.0f * dot;
	Collision col2 = Trace(col.position, col.normal);

	if (col2.scale == 1.#INF)
		return float4(BackLight(ray), 1.0f) * 0.9f;
	else {
		return float4(col2.material.emission, 1.0f) * 0.9f;
	}
}