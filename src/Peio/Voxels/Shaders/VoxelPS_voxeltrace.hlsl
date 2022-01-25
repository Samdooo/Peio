#ifndef PS_VOXELTRACE
#define PS_VOXELTRACE

#include "VoxelPS_input.hlsl"

struct VoxelRay {
    //float3 origin;
	//float3 ray;
    uint collisionVoxel;
    int side; // -1 indicates no collision
	float3 collision;
	float3 normal;
    Material material;
};

uint MaxIndex(float3 v) {
    if (v.x > v.y) {
        if (v.z > v.x)
            return 2;
        else
            return 0;
    }
    else {
        if (v.z > v.y)
            return 2;
        else
            return 1;
    }
}

VoxelRay VoxelTrace(const float3 origin, const float3 ray, uint skip){
    const uint numVoxels = scene[0].numVoxels;
    //const float voxelRadius = scene[0].voxelRadius;
    
    const float3 invRay = 1.0f / ray;
    const float3 invRad = abs(invRay * scene[0].voxelRadius);
    
    VoxelRay result;
    result.side = -1;
    result.normal = ray;

    float3 voxelOffset;
    float3 lowCorner;
    uint maxIndex;
    float high;
    float minScale = 1.#INF;
    
    [loop] for (uint voxel = 0; voxel < numVoxels; voxel++) {
        if (voxel == skip)
            continue;
        voxelOffset = (voxelPositions[voxel] - origin) * invRay;
        
        lowCorner = voxelOffset - invRad;
        maxIndex = MaxIndex(lowCorner);
        if (lowCorner[maxIndex] <= 0.0f || lowCorner[maxIndex] >= minScale)
            continue;
        
        if (min(min(voxelOffset.x + invRad.x, voxelOffset.y + invRad.y), voxelOffset.z + invRad.z) < lowCorner[maxIndex])
            continue;
        
        minScale = lowCorner[maxIndex];
        result.collisionVoxel = voxel;
        result.side = maxIndex;
    }
    if (result.side != -1) {
        result.collision = origin + ray * minScale;
        result.material = GetMaterial(result.collisionVoxel);
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
    //result.normal[result.side] = -result.normal[result.side]; // doesn't work for some reason
    return result;
}

#endif