#ifndef PS_LIGHTTRACE
#define PS_LIGHTTRACE

#include "VoxelPS_voxeltrace.hlsl"
#include "VoxelPS_sky.hlsl"
#include "VoxelPS_lightspread.hlsl"

float3 LightTrace(float3 origin, float3 startRay){
    const uint maxLayers = 30;
    const uint maxNumRays = scene[0].maxNumRays;
    const uint maxHitRays = scene[0].maxHitRays;
    
    VoxelRay ray[maxLayers];
    LightSpread spread[maxLayers - 1];
    float3 light[maxLayers - 1];
    light[0] = 0;
    
    for (uint i = 0; i < maxLayers - 1; i++){
        light[i] = 0.0f;
        ray[i].side = -1;
    }
    
    ray[0].collision = origin;
    ray[0].normal = startRay;
    ray[0].collisionVoxel = -1;
    ray[0].material.colorEmission = float4(1.0f, 1.0f, 1.0f, 1.0f);
    ray[0].material.lightEmission = 0;
    ray[0].material.lightSpread = 0;
    
    spread[0].Reset(startRay, 0.0f, -1, maxNumRays, maxHitRays);
    spread[0].ray = startRay;
    
    //if (!spread[0].Increment())
    //    return float3(1.0f, 0.0f, 0.0f);
    //else
    //    return float3(0.0f, 0.0f, 1.0f);
    
    uint c = 1;
    bool up = true;
    uint numRays = 0;
    uint numUps = 0;
    uint numDowns = 0;
    [loop] while (c > 0) {
        if (up){
            numUps++;
            ray[c] = VoxelTrace(ray[c - 1].collision, spread[c - 1].ray, ray[c - 1].collisionVoxel);
            
            numRays++;
            
            if (ray[c].side != -1) {
                light[c - 1] += ray[c].material.lightEmission;
                if (c < maxLayers - 1 && spread[c - 1].childrenRays > 0){
                    spread[c].Reset(ray[c].normal, ray[c].material.lightSpread, ray[c].side, spread[c - 1].childrenRays, maxHitRays);
                    light[c] = 0;
                    c++;
                    continue;
                }
            }
            if (ray[c].side == -1) {
                light[c - 1] += SkyTrace(spread[c - 1].ray);
            }
            if (spread[c - 1].Increment()){
                up = false;
                c--;
            }
        }
        if (!up) {
            numDowns++;
            if (c == 0)
                break;
            light[c] /= (float)spread[c].numRays;
            light[c] *= ray[c].material.colorEmission.rgb;
            light[c - 1] += light[c];
            if (spread[c - 1].Increment())
                c--;
            else
                up = true;
        }
    }
    //return float3((float)numRays / 255.0f, (float)numUps / 255.0f, (float)numDowns / 255.0f);
    //return float3((float)numRays / 255.0f, (float)numRays / 255.0f, (float)numRays / 255.0f);
    return light[0];
}

#endif