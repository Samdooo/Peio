#ifndef PS_INPUT
#define PS_INPUT

#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 1.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f

struct VSOutput {
    float4 pixelPosition : SV_POSITION;
    float3 cameraPosition : CAMERA_POSITION;
    float3 sightRay : SIGHT_RAY;
};

struct Scene {
    uint numVoxels;
    uint numMaterials;
    float voxelRadius;
    uint maxNumRays;
    uint maxHitRays;
    float minLightDeviation;
    float maxLightDeviation;
};
StructuredBuffer<Scene> scene : register(t0);

struct Material {
    float4 colorEmission;
	float3 lightEmission;
	float lightSpread;
};
StructuredBuffer<Material> materials : register(t1);

StructuredBuffer<float3> voxelPositions : register(t2);
StructuredBuffer<uint> voxelMaterials : register(t3);

Material GetMaterial(uint voxelIndex){
    //return materials[asuint(voxelMaterials[voxelIndex])];
    return materials[voxelMaterials[voxelIndex]];
}

#endif