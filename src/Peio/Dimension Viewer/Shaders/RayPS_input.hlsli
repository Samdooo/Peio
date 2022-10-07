#ifndef PS_INPUT
#define PS_INPUT

#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 0.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f
#define LIM (1.0f / (float)(1U << 16U))

#define numDims 3

struct Scene {
	uint numRands;
	uint2 windowSize;
	uint numRects;
	uint numSpheres;
	float3 backLight;
	uint numBounces;
};
StructuredBuffer<Scene> scene : register(t0);

struct Camera {
    float position[numDims];
    float rotation[numDims - 1];
    float fov;
};
StructuredBuffer<Camera> camera : register(t1);

StructuredBuffer<uint> raysPerBounce : register(t2);

struct Material {
	float3 emission;
	float3 reflection;
	float spectral;
};

#endif