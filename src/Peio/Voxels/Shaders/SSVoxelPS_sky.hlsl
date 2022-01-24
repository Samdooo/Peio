#ifndef PS_SKY
#define PS_SKY

#include "SSVoxelPS_input.hlsl"

float3 SkyTrace(float3 ray){
    const float3 sunRay = float3(1.0f, 1.0f, 1.0f);
    ray = normalize(ray);
    
    float3 diff = length(ray - sunRay);
    return float3(0.5f, 0.8f, 0.9f) + float3(2.0f, 2.0f, 1.8f) * max((1.0f - diff), 0.0f);
    
    //return float3(0.1f, 0.1f, 0.15f) * ((ray.y + 1.0f) / 2.0f);
    //return 0.0f;
}

#endif