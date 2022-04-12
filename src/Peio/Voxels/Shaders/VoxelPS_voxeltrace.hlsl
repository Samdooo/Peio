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
    
    const uint numLayers = 14;
    const uint numChildren = 3;
    const float3 invRay = 1.0f / ray;
    const float3 invRad = abs(invRay * scene[0].voxelRadius);
    
    VoxelRay result;
    result.side = -1;
    result.normal = ray;
    
    float minScale = 1.#INF;
    uint layerSize = numChildren; // In nodes
    uint layerOffset = 0;
    uint layerIndex = 0;
    uint nodeIndex = 0;
    
    uint branchIndices[numLayers];
    [unroll(numLayers)] for (uint i = 0; i < numLayers; i++)
        branchIndices[i] = 0;
    
    uint descriptors[numLayers];
    descriptors[0] = -1;
    
    /*
    for (uint i = 0; i < scene[0].numVoxels; i++){
        float3 voxel = voxelPositions[i];
        
        voxel -= origin;
        voxel *= invRay;
           
        float curMax = min(voxel.x + invRad.x, min(voxel.y + invRad.y, voxel.z + invRad.z));
        if (curMax <= 0.0f)
            continue;
        float curMin = max(voxel.x - invRad.x, max(voxel.y - invRad.y, voxel.z - invRad.z));
        
        if (curMax < curMin || curMin >= minScale || curMin <= 0.0f)
            continue;
        
        minScale = curMin;
        result.collisionVoxel = positionLeaves[nodeIndex + i].voxelIndex;
        
        result.side = 0;
        [unroll(2)] for(int i = 1; i < 3; i++) {
            if ((voxel[i] - invRad[i]) > (voxel[result.side] - invRad[result.side]))
                result.side = i;
        }
    }
    for (uint i = 0; i < numChildren; i++){
        if (!(descriptors[0] & (1 << i)))
            continue;
        PositionBranch branch = positionBranches[i];
        branch.lowerBound -= origin;
        branch.higherBound -= origin;
        
        branch.lowerBound *= invRay;
        branch.higherBound *= invRay;
        
        float curMax = min(max(branch.lowerBound.x, branch.higherBound.x), 
                       min(max(branch.lowerBound.y, branch.higherBound.y), 
                           max(branch.lowerBound.z, branch.higherBound.z)));
        if (curMax <= 0.0f)
            continue;
        float curMin = max(min(branch.lowerBound.x, branch.higherBound.x), 
                       max(min(branch.lowerBound.y, branch.higherBound.y), 
                           min(branch.lowerBound.z, branch.higherBound.z)));
        
        if (curMax < curMin || curMin >= minScale)
            continue;
            
        result.side = 0;
        result.collisionVoxel = 0;
        break;
    }
    */

    uint maxLayer = 0;
    bool up = true;
    uint checks = 0;

    [loop] while (true) {
        checks++;
        if (layerIndex > maxLayer)
            maxLayer = layerIndex;
        if (layerIndex == numLayers - 1) {
            [unroll(numChildren)] for (uint v = 0; v < numChildren; v++) {
                if (!(descriptors[layerIndex] & (1U << (v * 2))))
                    continue;
                float3 voxel = voxelPositions[positionLeaves[nodeIndex + v].voxelIndex];
                voxel -= origin;
                voxel *= invRay;
                
                float curMax = min(voxel.x + invRad.x, min(voxel.y + invRad.y, voxel.z + invRad.z));
                if (curMax <= 0.0f)
                    continue;
                float curMin = max(voxel.x - invRad.x, max(voxel.y - invRad.y, voxel.z - invRad.z));
                
                if (curMax < curMin || curMin >= minScale || curMin <= 0.0f)
                    continue;
                
                minScale = curMin;
                result.collisionVoxel = positionLeaves[nodeIndex + v].voxelIndex;
                
                result.side = 0;
                [unroll(2)] for(int j = 1; j < 3; j++) {
                    if ((voxel[j] - invRad[j]) > (voxel[result.side] - invRad[result.side]))
                        result.side = j;
                }
            }
            up = false;
        }
        else {
            up = false;
            [unroll(numChildren)] for (; branchIndices[layerIndex] < numChildren; branchIndices[layerIndex]++) {
                if (!(descriptors[layerIndex] & (1U << (branchIndices[layerIndex] * 2))))
                    continue;
                PositionBranch branch = positionBranches[layerOffset + nodeIndex + branchIndices[layerIndex]];
                
                branch.lowerBound -= origin;
                branch.higherBound -= origin;
                
                branch.lowerBound *= invRay;
                branch.higherBound *= invRay;
                
                float curMax = min(max(branch.lowerBound.x, branch.higherBound.x), 
                               min(max(branch.lowerBound.y, branch.higherBound.y), 
                                   max(branch.lowerBound.z, branch.higherBound.z)));
                if (curMax <= 0.0f)
                    continue;
                float curMin = max(min(branch.lowerBound.x, branch.higherBound.x), 
                               max(min(branch.lowerBound.y, branch.higherBound.y), 
                                   min(branch.lowerBound.z, branch.higherBound.z)));
                
                if (curMax < curMin || curMin >= minScale)
                    continue;
                
                nodeIndex += branchIndices[layerIndex];
                descriptors[layerIndex + 1] = branch.descriptor;
                branchIndices[layerIndex]++;
                branchIndices[layerIndex + 1] = 0;
                up = true;
                break;
            }
        }
        
        if (up){
            layerIndex++;
            layerOffset += layerSize;
            layerSize *= numChildren;
            nodeIndex *= numChildren;
        }
        else {
            if (layerIndex == 0)
                break;
            layerIndex--;
            layerSize /= numChildren;
            layerOffset -= layerSize;
            nodeIndex /= numChildren * numChildren;
            nodeIndex *= numChildren;
        }
    }
    //if (result.side != -1)
    //    maxLayer++;
    //if (maxLayer != 0) {
    //    result.side = 0;
    //    float f = (float)maxLayer / (float)numLayers;
    //    result.material.colorEmission = float4(f, (float)checks / (float)256 / (float)100, (float)checks / (float)256 / (float)1000, 1.0f);
    //    return result;
    //}

    if (result.side != -1){
        result.collision = origin + (ray * minScale);
        result.normal = ray;
        if (result.side == 0)
            result.normal.x = -result.normal.x;
        else if (result.side == 1) 
            result.normal.y = -result.normal.y;
        else
            result.normal.z = -result.normal.z;
        result.material = materials[voxelMaterials[result.collisionVoxel]];
        result.material.colorEmission *= float(result.side + 1) / 3.0f;
    }
    return result;
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