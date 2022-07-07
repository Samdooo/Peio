#ifndef PS_SKY
#define PS_SKY

#include "VoxelPS_input.hlsl"

float3 SkyTrace(float3 ray) {
    const float3 sunRay = float3(1.0f, 1.0f, 1.0f);
    ray = normalize(ray);

    float3 diff = length(ray - sunRay);
    return float3(0.5f, 0.8f, 0.9f) + float3(20.0f, 20.0f, 18.0f) * max((1.0f - diff), 0.0f);
}

#endif