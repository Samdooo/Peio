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

struct Sky {
    float2 sunRotation;
};

struct Scene {
    uint numRays;
    uint2 windowSize;
    Sky sky;
    bool newRays;
};

StructuredBuffer<Scene> scene : register(t1);

struct MaterialGroup {
    uint indices[2][2][2];
};

StructuredBuffer<MaterialGroup> tree : register(t2);

struct Material {
    float4 color;
    float3 light;
    float spread;
};

StructuredBuffer<Material> materials : register(t3);

struct Ray {
    uint3 voxel;
    uint material;
    uint side;
    float3 light;
    uint numRays;
};

RWStructuredBuffer<Ray> rays : register(u1);

RWStructuredBuffer<uint> random : register(u2);
#define MAX_SEED (1 << 10)

float randFloat(uint2 pixelPosition, uint add) {
    uint seed = random[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];
    return abs(frac(cos((float)(seed + add) * PHI) * 12345.6789f));
}

#endif