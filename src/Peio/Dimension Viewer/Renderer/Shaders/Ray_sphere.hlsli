#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Ray_math.hlsli"
#include "Ray_scene.hlsli"

struct Sphere {
	float center[numDims];
	float radius;
	Material material;
};
StructuredBuffer<Sphere> spheres : register(t3);

bool TraceSphere(uint ind, in float origin[numDims], in float ray[numDims], in out float scale){
	Sphere sphere = spheres[ind];
	
	float a = 0.0f, b = 0.0f, c = 0.0f;
	[unroll(numDims)] for (uint i = 0; i < numDims; i++){
		a += ray[i] * ray[i];
		float d = (origin[i] - sphere.center[i]);
		b += 2.0f * ray[i] * d;
		c += d * d - sphere.radius * sphere.radius;
	}
	
	float sqr = b * b - 4 * a * c;
	if (sqr < limit)
		return false;
	float aInv = 1.0f / a;
	float left = -b * aInv * 0.5f;
	float right = abs(sqrt(sqr) * aInv * 0.5f);
	
	float sMax = left + right;
	if (sMax < limit)
		return false;
	float sMin = left - right;
	if (sMin >= scale)
		return false;
	if (sMin > limit)
        scale = sMin;
    else
        scale = sMax;
	return true;
}

void NormalSphere(uint ind, in float p[numDims], out float normal[numDims]){
	const Sphere sphere = spheres[ind];
	[unroll(numDims)] for (uint i = 0; i < numDims; i++)
        normal[i] = 2.0f * (p[i] - sphere.center[i]);
}

#endif