#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
    //uint uints[2];
    //float4 light[2];
    //uints[0] = 0;
    //uints[1] = 0;
    //uint c = 1;
    //[loop] while (c > 0) {
    //    if (uints[c] == 0)
    //        light[c - 1] = 0.0f;
    //    if (uints[c] != 0)
    //        light[c - 1] = 1.0f;
    //    c--;
    //    break;
    //}
    //return light[0];

    //VoxelRay ray1 = VoxelTrace(input.cameraPosition, input.sightRay);

    //float4 light;
    //VoxelRay ray;
    //
    //float3 cameraPosition = 0.0f;
    //float3 sightRay = 0.0f;
    //if (scene[0].useScene == 1){
    //    cameraPosition = input.cameraPosition;
    //    sightRay = input.sightRay;
    //}
    //
    //for (uint i = 0; i < scene[0].numMaterials; i++){
    //    ray = VoxelTrace(cameraPosition, sightRay, -1);
    //    light = ray.material.colorEmission;
    //}
    //return light;
    //return float4(LightTrace(input.cameraPosition, input.sightRay), 1.0f);
    return float4(SkyTrace(input.sightRay), 1.0f);
}