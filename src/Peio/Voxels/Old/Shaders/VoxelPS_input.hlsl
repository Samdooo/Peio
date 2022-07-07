#ifndef PS_INPUT
#define PS_INPUT

#define SQRT_HALF 0.70710678118654f
#define SQRT_3_4 0.86602540378443f
#define SQRT_2 1.41421356237309f
#define FLOAT_MAX 3.402823466e+38f
#define PI 3.14159265358979f
#define PHI 0.618033988749895f
#define GOLDEN_ANGLE 2.399963229728653f

struct VSOutput {
    float4 pixelPosition : SV_POSITION;
    float3 cameraPosition : CAMERA_POSITION;
    float3 sightRay : SIGHT_RAY;
};

// SRVs

struct Scene {
    uint2 screenSize;
    uint numVoxels;
    uint numMaterials;
    float voxelRadius;
    uint numRays;
};
StructuredBuffer<Scene> scene : register(t0);

struct Material {
    float4 colorEmission;
	float3 lightEmission;
};
StructuredBuffer<Material> materials : register(t1);

StructuredBuffer<float3> voxelPositions : register(t2);
StructuredBuffer<uint> voxelMaterials : register(t3);

struct PositionBranch {
    uint descriptor;
    float3 lowerBound, higherBound;
};
struct PositionLeaf {
    uint voxelIndex;
};

StructuredBuffer<PositionBranch> positionBranches : register(t4);
StructuredBuffer<PositionLeaf> positionLeaves : register(t5);

Material GetMaterial(uint voxelIndex){
    return materials[voxelMaterials[voxelIndex]];
}

// UAVs

struct PrimaryRay {
    uint materialIndex;
    int side; // -1 indicates no collision
    float3 collision;
    float3 light;
};

RWStructuredBuffer<PrimaryRay> primaryRays : register(u1);

#endif