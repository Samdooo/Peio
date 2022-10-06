#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 0.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f

struct VSOutput {
	float4 position : SV_POSITION;
};

struct Scene {
	uint numDims;
	uint2 windowSize;
	uint numRects;
	uint numSpheres;
	float3 backLight;
	uint numBounces;
};
StructuredBuffer<Scene> scene : register(t0);

StructuredBuffer<float> camera : register(t1);

float CamPos(uint d) {
	return camera[d];
}
float CamRot(uint d) {
	return camera[scene[0].numDims + d];
}
float CamFov() {
	return camera[scene[0].numDims + scene[0].numDims - 1];
}
float CamCorner(uint c, uint d) {
	return camera[scene[0].numDims + scene[0].numDims + (c * scene[0].numDims) + d];
}

StructuredBuffer<uint> raysPerBounce : register(t2);

struct Material {
	float3 emission;
	float3 reflection;
	float spectral;
};

StructuredBuffer<float> hyperRects : register(t3);
StructuredBuffer<Material> hyperRectMaterials : register(t4);

float HyperRectPos(uint i, uint d) {
	return hyperRects[i * (scene[0].numDims * 2) + d];
}
float HyperRectSize(uint i, uint d) {
	return hyperRects[i * (scene[0].numDims * 2) + scene[0].numDims + d];
}

StructuredBuffer<float> hyperSpheres : register(t5);
StructuredBuffer<Material> hyperSphereMaterials : register(t6);

float HyperSphereCenter(uint i, uint d) {
	return hyperSpheres[i * (scene[0].numDims + 1) + d];
}
float HyperSphereRadius(uint i) {
	return hyperSpheres[i * (scene[0].numDims + 1) + scene[0].numDims];
}

StructuredBuffer<uint> seeds : register(t7);
StructuredBuffer<float> randoms : register(t8);

float Rand(uint2 pixelPositon, uint add) {
	return randoms[(seeds[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x] + add) % numRandoms];
}

float4 main(VSOutput input) : SV_TARGET
{
	uint2 pixelPosition = (uint2)input.position.xy;
	randIndex = seeds[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];

}