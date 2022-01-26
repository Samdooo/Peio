#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    return float4(LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition), 1.0f);
    //float3 light = LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition);
    //return float4(light, 1.0f);
    
    //const uint numSamples = 1;s
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