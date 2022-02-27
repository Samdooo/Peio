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

VoxelRay VoxelTrace(const float3 origin, const float3 ray, uint skip){
    
    const uint numLayers = 10;
    const uint branchSize = 7;
    const float3 invRay = 1.0f / ray;
    const float3 invRad = abs(invRay * scene[0].voxelRadius);
    
    bool3 rayNeg = bool3(ray.x < 0.0f, ray.y < 0.0f, ray.z < 0.0f);
    
    VoxelRay result;
    result.side = -1;
    result.normal = ray;
    
    float minScale = 1.#INF;
    
    uint descriptor[2];
    uint descriptorIndex = 0;
    
    uint layerIndex = 0, index = 0;
    uint layerSize = 3;
    uint layerOffset = 0;
    
    bool up = true;
    [loop] while (true) {
        if (up){
            if (layerIndex == numLayers - 1){
                // Check the objects
                
                
                
            }
            else {
                // Zero the "two checked" descriptor bit for current layer
                descriptor &= ~(1 << (2 * layerIndex + 1));
                
                // Sort by scale
                float3 scales = 0;
                [unroll] for (uint i = 0; i < 3; i++){
                    uint offset = (layerOffset + index + i) * branchSize + 1;
                    float3 minPoint = float3(asfloat(positionBranches[offset]), asfloat(positionBranches[offset + 1]), asfloat(positionBranches[offset + 2]));
                    float3 maxPoint = float3(asfloat(positionBranches[offset + 3]), asfloat(positionBranches[offset + 4]), asfloat(positionBranches[offset + 5]));
                    
                    [unroll] for (uint j = 0; j < 3; j++){
                        if (rayNeg[j]){
                            
                        }
                        else {
                            
                        }
                    }
                    
                }
            }
        }
        else {
            
        }
        if (up){
            layerOffset += layerSize;
            layerSize *= 3;
            index *= 3;
            layerIndex++;
            if ((layerIndex & 7) == 0)
                descriptorIndex++;
        }
        else {
            layerOffset -= layerSize;
            layerSize /= 3;
            index /= 3;
            layerIndex--;
            if ((layerIndex & 7) == 7)
                descriptorIndex--;
        }
    }
    
}

//uint MaxIndex(float3 v) {
//    if (v.x > v.y) {
//        if (v.z > v.x)
//            return 2;
//        else
//            return 0;
//    }
//    else {
//        if (v.z > v.y)
//            return 2;
//        else
//            return 1;
//    }
//}
//
//VoxelRay VoxelTrace(const float3 origin, const float3 ray, uint skip){
//    const uint numVoxels = scene[0].numVoxels;
//    //const float voxelRadius = scene[0].voxelRadius;
//    
//    const float3 invRay = 1.0f / ray;
//    const float3 invRad = abs(invRay * scene[0].voxelRadius);
//    
//    VoxelRay result;
//    result.side = -1;
//    result.normal = ray;
//
//    float3 voxelOffset;
//    float3 lowCorner;
//    uint maxIndex;
//    float high;
//    float minScale = 1.#INF;
//    
//    [loop] for (uint voxel = 0; voxel < numVoxels; voxel++) {
//        if (voxel == skip)
//            continue;
//        voxelOffset = (voxelPositions[voxel] - origin) * invRay;
//        
//        lowCorner = voxelOffset - invRad;
//        maxIndex = MaxIndex(lowCorner);
//        if (lowCorner[maxIndex] <= 0.0f || lowCorner[maxIndex] >= minScale)
//            continue;
//        
//        if (min(min(voxelOffset.x + invRad.x, voxelOffset.y + invRad.y), voxelOffset.z + invRad.z) < lowCorner[maxIndex])
//            continue;
//        
//        minScale = lowCorner[maxIndex];
//        result.collisionVoxel = voxel;
//        result.side = maxIndex;
//    }
//    if (result.side != -1) {
//        result.collision = origin + ray * minScale;
//        result.material = GetMaterial(result.collisionVoxel);
//    }
//    switch (result.side) {
//    case 0:
//        result.normal.x = -result.normal.x;
//        break;
//    case 1:
//        result.normal.y = -result.normal.y;
//        break;
//    case 2:
//        result.normal.z = -result.normal.z;
//        break;
//    }
//    //result.normal[result.side] = -result.normal[result.side]; // doesn't work for some reason
//    return result;
//}

#endif