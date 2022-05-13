#include "VoxelPS_input.hlsl"
//#include "VoxelPS_voxeltrace.hlsl"
//#include "VoxelPS_sky.hlsl"

RWStructuredBuffer<float4> uavTexture : register(u1);

float4 main(VSOutput input) : SV_TARGET{
    float4 col = uavTexture[(uint)input.pixelPosition.y * 640 + (uint)input.pixelPosition.x];
    uavTexture[(uint)input.pixelPosition.y * 640 + (uint)input.pixelPosition.x] *= 0.99 + 0.005f * ((320.0f - abs(320.0f - input.pixelPosition.x)) / 320.0f) + 0.005f * ((180.0f - abs(180.0f - input.pixelPosition.y)) / 180.0f);
    return col;
    //float4 col = uavTexture[(uint)input.pixelPosition.y * 640 + (uint)input.pixelPosition.x];
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //VoxelRay ray = VoxelTrace(input.cameraPosition, input.sightRay, -1);
    //if (ray.side == -1){
    //    return float4(SkyTrace(input.sightRay), 1.0f);
    //}
    //else {
    //    return ray.material.colorEmission;
    //}
}