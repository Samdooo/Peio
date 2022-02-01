#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    VoxelRay ray;
    float3 light = LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition, ray);
    if (ray.side == -1)
        return float4(light, 1.0f);
    
    switch (ray.side) {
    case 0:
        if (light.r == 0.0f)
            light.r = 1.#INF;
        else
            light.r = -light.r;
        break;
    case 1:
        if (light.g == 0.0f)
            light.g = 1.#INF;
        else
            light.g = -light.g;
        break;
    case 2:
        if (light.r == 0.0f)
            light.r = 1.#INF;
        else
            light.r = -light.r;
        if (light.g == 0.0f)
            light.g = 1.#INF;
        else
            light.g = -light.g;
        break;
    }

    return float4(light, ray.collision[ray.side]);

    //const uint numSamples = 1;
    //float3 light = 0.0f;
    //for (uint s = 0; s < numSamples; s++) {
    //    light += LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition) / (float)numSamples;
    //    if (s == 0)
    //        input.pixelPosition.x += 1.0f;
    //    else if (s == 1)
    //        input.pixelPosition.y += 1.0f;
    //    else if (s == 2)
    //        input.pixelPosition.x -= 1.0f;
    //}
    //return float4(light, 1.0f);
}