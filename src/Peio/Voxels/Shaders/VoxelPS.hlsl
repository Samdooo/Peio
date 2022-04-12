#include "VoxelPS_voxeltrace.hlsl"
#include "VoxelPS_sky.hlsl"

float4 main(VSOutput input) : SV_TARGET {
    VoxelRay ray = VoxelTrace(input.cameraPosition, input.sightRay, -1);
    if (ray.side == -1){
        return float4(SkyTrace(input.sightRay), 1.0f);
    }
    else {
        return ray.material.colorEmission;
    }
}