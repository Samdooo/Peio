#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    return float4(LightTrace(input.cameraPosition, input.sightRay), 1.0f);
}