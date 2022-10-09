#ifndef S_INPUT
#define S_INPUT

#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 0.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f
#define LIM (1.0f / (float)(1U << 14U))

#define numDims 3
#define maxBounces 4

struct Vector {
	float v[numDims];
};
struct Rotation {
	float v[numDims - 1];
};

struct VSOutput {
	float4 position : SV_POSITION;
	Vector ray : VS_RAY;
};

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
    Vector position;
    Rotation rotation;
    float fov;
	float aspectRatio;
};
StructuredBuffer<Camera> camera : register(t1);

StructuredBuffer<uint> raysPerBounce : register(t2);

struct Material {
	float3 emission;
	float3 reflection;
	float spectral;
};

struct NoiseRay {
	//Vector position;
	//Material material;
	float scale;
	Vector normal;
	float3 diffuseLight;
	float3 reflection;
	float3 emission;
	float spectral;
};
RWStructuredBuffer<NoiseRay> noiseRays : register(u1);

#endif