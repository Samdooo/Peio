#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    float3 light = LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition);
    primaryRays[(uint)input.pixelPosition.y * scene[0].screenSize.x + (uint)input.pixelPosition.x].light = light;
    return 0.0f;//float4(light, 1.0f);
}