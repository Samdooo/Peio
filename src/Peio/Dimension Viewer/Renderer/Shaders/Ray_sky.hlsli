#include "Ray_scene.hlsli"

float3 SkyTrace(in float ray[numDims]){
    float3 top = float3(0.31f, 0.95f, 0.89f);
    float3 bottom = float3(0.05f, 0.1f, 0.2f);
    
    Normalize(ray);
    float topRay[numDims];
    [unroll(numDims)] for (uint i = 0; i < numDims; i++)
        topRay[i] = 0.0f;
    topRay[1] = 1.0f;

    float dot = Dot(topRay, ray);
    
    return bottom + max(top * (dot + 0.5f), float3(0.0f, 0.0f, 0.0f));
}