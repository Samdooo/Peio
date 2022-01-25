#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    const uint numSamples = 4;
    float3 light = 0.0f;
    for (uint s = 0; s < numSamples; s++) {
        light += LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition) / (float)numSamples;
        if (s == 0)
            input.pixelPosition.x += 1.0f;
        else if (s == 1)
            input.pixelPosition.y += 1.0f;
        else if (s == 2)
            input.pixelPosition.x -= 1.0f;
    }
    return float4(light, 1.0f);
}