#ifndef PS_SPHERE
#define PS_SPHERE

#include "RayPS_input.hlsli"

struct HyperSphere {
	float center[numDims];
	float radius;
	Material material;
};
StructuredBuffer<HyperSphere> hyperSpheres : register(t4);

bool TraceSphere(float origin[numDims], float ray[numDims], in out float scale, out uint outSphere, out float normal[numDims]){
    const uint numSpheres = scene[0].numSpheres;
    
    bool hit = false;
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
		
		if (sMax <= LIM)
			continue;
            
        float s = sMin;
		if (sMin <= LIM)
			s = sMax;
        if (s < scale){
			scale = s;
            outSphere = i;
            hit = true;
		}
    }
    if (hit){
        [unroll] for (uint i = 0; i < numDims; i++)
            normal[i] = (origin[i] + ray[i] * scale) - hyperSpheres[outSphere].center[i];
    }
    return hit;
}

#endif