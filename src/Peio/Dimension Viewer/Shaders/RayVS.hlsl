#define numDims 3

void Rot(out float pos[numDims], float vs[numDims - 1]) {
	/* Weird error with [unroll] */ [loop] for (uint i = 0; i < numDims - 1; i++) {
		float x = pos[0] * cos(vs[i]) - pos[i + 1] * sin(vs[i]);
		float y = pos[i + 1] * cos(vs[i]) + pos[0] * sin(vs[i]);
		pos[0] = x;
		pos[i + 1] = y;
	}
}

struct VSOutput {
	float4 position : SV_POSITION;
	float ray[numDims] : VS_RAY;
};

struct Camera {
	float position[numDims];
	float rotation[numDims - 1];
	float fov;
	float aspectRatio;
};
StructuredBuffer<Camera> camera : register(t0);

float3 RotateX(float3 p, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return float3(p.x, c * p.y + s * p.z, -s * p.y + c * p.z);
}

float3 RotateY(float3 p, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return float3(c * p.x - s * p.z, p.y, s * p.x + c * p.z);
}

VSOutput main(float2 position : POSITION)
{
	VSOutput output;
	output.position = float4(position, 0.0f, 1.0f);

	//output.ray[0] = position.x;
	//output.ray[1] = position.y * camera[0].aspectRatio;
	//output.ray[2] = 1.0f / tan(camera[0].fov / 2.0f);
	output.ray[0] = 1.0f / tan(camera[0].fov / 2.0f);
	output.ray[1] = position.y * camera[0].aspectRatio;
	output.ray[2] = position.x;
	for (uint i = 3; i < numDims; i++)
		output.ray[i] = 0.0f;
	//Rot(output.ray, camera[0].rotation);
	[loop] for (uint i = 0; i < numDims - 1; i++) {
		float x = output.ray[0] * cos(camera[0].rotation[i]) - output.ray[i + 1] * sin(camera[0].rotation[i]);
		float y = output.ray[i + 1] * cos(camera[0].rotation[i]) + output.ray[0] * sin(camera[0].rotation[i]);
		output.ray[0] = x;
		output.ray[i + 1] = y;
	}

	return output;
}