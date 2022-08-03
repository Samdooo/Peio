#ifndef PS_VOXELTRACE
#define PS_VOXELTRACE

#include "VoxelPS_input.hlsl"

struct VoxelRay {
    uint3 voxel;
    uint side;
    float3 normal;
    uint material;
    float3 collision;
};

VoxelRay VoxelTrace(const float3 origin, const float3 inRay, uint3 skip) {

    const float3 ray = inRay;//normalize(inRay);

    const uint numLayers = 16;
    const float3 invRay = 1.0 / ray;

    VoxelRay result;
    result.voxel = 0;
    result.side = ~0;
    result.normal = 0.0;
    result.material = ~0;
    
    float curScale = 0.0;
    const float minTotalScale = 0.001;

    { // Trace to the root cube
        float rad = (float)(1U << numLayers) / 2.0;
        float3 mid = float3(rad, rad, rad) - origin;
        float3 minDiv = mid * invRay - abs(invRay * rad);
        float3 maxDiv = mid * invRay + abs(invRay * rad);

        //float minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0);
        float minScale = 0.0;
        [unroll(3)] for (uint i = 0; i < 3; i++) {
            if (minDiv[i] > minScale) {
                minScale = minDiv[i];
                result.side = i;
            }
        }

        float maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);

        if (minScale > maxScale) {
            result.side = ~0;
            return result;
        }
        curScale = minScale;
    }
    
    uint3 path = 0;
    uint curLayer = 0;
    uint indices[numLayers];
    uint nextIndex = 0;
    bool down = true;

    //float lodMul = 512.0f;

    [loop] while (true) {
        uint mask = 1U << (numLayers - curLayer - 1);
        float rad = (float)(1U << (numLayers - curLayer)) / 2.0;
        float3 mid = (float3)(path & uint3(~mask, ~mask, ~mask)) + float3(rad, rad, rad);

        if (down) {
            if (curLayer == numLayers) {
                //if (path.x == skip.x && path.y == skip.y && path.z == skip.z) {
                //    curLayer--;
                //    down = false;
                //    continue;
                //}
                if (curScale < minTotalScale) {
                    curLayer--;
                    down = false;
                    continue;
                }
                result.voxel = path;
                result.material = nextIndex;
                break;
            }

            float3 curPos = origin + ray * curScale;
            indices[curLayer] = nextIndex;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                if (curPos[i] >= mid[i]/*&& curScale < rad * lodMul*/)
                    path[i] |= mask;
                else
                    path[i] &= ~mask;
            }
        }
        else {
            uint axis = -1;
            float bestScale = 1.#INF;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                float s = (mid[i] - origin[i]) * invRay[i];
                if (s <= curScale) // Might cause errors because of float inaccuracy (?)
                    continue;
                if (s < bestScale) {
                    axis = i;
                    bestScale = s;
                }
            }

            bool inside = true;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                if (abs((origin[i] + ray[i] * bestScale) - mid[i]) > rad) {
                    inside = false;
                    break;
                }
            }

            if (axis == -1 || !inside) {
                if (curLayer == 0)
                    break;
                [unroll(3)] for (uint i = 0; i < 3; i++)
                    path &= ~mask;
                curLayer--;
                continue;
            }
            result.side = axis;
            curScale = bestScale;
            path[axis] ^= mask;
        }
        nextIndex = tree[indices[curLayer]].indices[(bool)(path.x & mask)]
            [(bool)(path.y & mask)][(bool)(path.z & mask)];
        if (nextIndex == -1) {
            down = false;
            continue;
        }
        else {
            down = true;
            curLayer++;
        }
    }
    if (result.material != ~0) {
        result.normal = (float3)ray;
        result.normal[result.side] = -result.normal[result.side];
        result.collision = (float3)origin + (float3)(ray * curScale);
        result.collision[result.side] = (float)result.voxel[result.side];
        if (ray[result.side] < 0.0)
            result.collision[result.side] += 1.01f;
        else
            result.collision[result.side] -= 0.01f;
        //else
        //    result.collision[result.side] -= 0.5f;
    }
    return result;
}


#endif