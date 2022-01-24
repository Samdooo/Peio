#ifndef PS_VOXELTRACE
#define PS_VOXELTRACE

#include "SSVoxelPS_input.hlsl"

struct VoxelRay {
    //float3 origin;
	//float3 ray;
    uint collisionVoxel;
    int side; // -1 indicates no collision
	float3 collision;
	float3 normal;
    Material material;
};

VoxelRay VoxelTrace(const float3 origin, const float3 inRay, uint skip){
    uint numVoxels = 0;
    float voxelRadius = 0.0f;
    if (scene[0].useScene == 1){
        numVoxels = scene[0].numVoxels;
        voxelRadius = scene[0].voxelRadius;
    }
    
    const float3 ray = normalize(inRay);
    float3 invRay = 1.0f / ray;
    
    VoxelRay result;
    //result.origin = origin;
    //result.ray = ray;
    result.collisionVoxel = 0;
    result.side = -1;
    result.collision = 0;
    result.normal = 0;
    
    result.material.colorEmission = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float3 voxelOffset;
    float scale;
    float3 traced;
    float3 tracedDiff;
    bool hits;
    float minScale = FLOAT_MAX;
    
    uint checked = 0;
    [loop] for (uint voxel = 0; voxel < numVoxels; voxel++){
        if (voxel == skip)
            continue;
        voxelOffset = voxelPositions[voxel] - origin;//float3(1.0f, 2.0f, 3.0f);
        
        if (length(voxelOffset) - SQRT_3_4 >= minScale)
            continue;
        
        checked++;
        //if (length(voxelOffset) == 1000.0f){
        //    result.material.colorEmission = float4(0.0f, 1.0f, 0.0f, 1.0f);
        //}
        //continue;
        
        //if (voxelOffset.x == 1000){
        //    result.material.colorEmission = float4(0.0f, 1.0f, 0.0f, 1.0f);
        //}
        //continue;
        
        [unroll(3)] for (uint side = 0; side < 3; side++){
            scale = (voxelOffset[side] + ((voxelOffset[side] < 0.0f) ? voxelRadius : -voxelRadius)) * invRay[side];
            
            if (scale <= 0.0f || scale >= minScale)
                continue;
                
            traced = ray * scale;
            tracedDiff = abs(traced - voxelOffset);
            
            //if (side == 0)
            //    if (tracedDiff.y > scene[0].voxelRadius || tracedDiff.z > scene[0].voxelRadius)
            //        continue;
            //else if (side == 1)
            //    if (tracedDiff.x > scene[0].voxelRadius || tracedDiff.z > scene[0].voxelRadius)
            //        continue;
            //else if (side == 2)
            //    if (tracedDiff.x > scene[0].voxelRadius || tracedDiff.y > scene[0].voxelRadius)
            //        continue;
            hits = true;
            [unroll(3)] for (uint axis = 0; axis < 3; axis++) {
                //if (axis != side && abs(traced[axis] - voxelOffset[axis]) > scene[0].voxelRadius) {
                //    hits = false;
                //    break;
                //}
                if (axis != side && tracedDiff[axis] > voxelRadius) {
                    hits = false;
                    break;
                }
            }
            if (hits) {
                minScale = scale;
                result.collisionVoxel = voxel;
                //result.material.colorEmission = float4(0.0f, 1.0f, 0.0f, 1.0f);
                result.material = GetVoxelMaterial(voxel);
                result.collision = origin + traced;
                result.side = side;
                result.normal = ray;
            }
        }
    }
    switch (result.side) {
    case 0:
        result.normal.x = -result.normal.x;
        break;
    case 1:
        result.normal.y = -result.normal.y;
        break;
    case 2:
        result.normal.z = -result.normal.z;
        break;
    }
    result.material.colorEmission.g = (float)checked / 255.0f;
    //result.normal[result.side] = -result.normal[result.side]; // doesn't work for some reason
    return result;
}

#endif