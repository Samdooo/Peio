#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "Ray_header.hlsli"

struct Denoiser {
	uint radius;
	float maxDist;
	float maxAngle;
};

struct Scene {
	uint2 screenSize;
	uint numRects;
	uint numSpheres;
	uint numRandom;
	Denoiser denoiser;
};

StructuredBuffer<Scene> scene : register(t0);

struct Camera {
	float position[numDims];
	float rotation[numDims - 1];
	float fov;
	float aspectRatio;
};
StructuredBuffer<Camera> camera : register(t1);

struct RandomRay {
	float ray[numDims];
};
StructuredBuffer<RandomRay> randomRays : register(t4);

StructuredBuffer<uint> raysPerBounce : register(t5);

static uint pixInd = 0;
struct Pixel {
	uint seed;
};
RWStructuredBuffer<Pixel> pixels : register(u1);

struct DenoisePixel {
	float position[numDims];
	float normal[numDims];
	float3 light;
};
RWStructuredBuffer<DenoisePixel> denoisePixels : register(u2);

struct Material {
    float3 emission;
	float3 reflection;
	float specular;
};

struct TracedRay {
	float collision[numDims];
	uint2 obj; // (type, index)
};

#endif