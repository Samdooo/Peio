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

VoxelRay VoxelTrace(const double3 origin, const double3 ray, uint3 skip) {

    const uint numLayers = 12;
    const double3 invRay = 1.0 / ray;

    VoxelRay result;
    result.voxel = 0;
    result.side = ~0;
    result.normal = 0.0;
    result.material = ~0;
    
    double curScale = 0.0;
    const double minTotalScale = 0.001;

    { // Trace to the root cube
        double rad = (double)(1U << numLayers) / 2.0;
        double3 mid = double3(rad, rad, rad) - origin;
        double3 minDiv = mid * invRay - abs(invRay * rad);
        double3 maxDiv = mid * invRay + abs(invRay * rad);

        //double minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0);
        double minScale = 0.0;
        [unroll(3)] for (uint i = 0; i < 3; i++) {
            if (minDiv[i] > minScale) {
                minScale = minDiv[i];
                result.side = i;
            }
        }

        double maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);

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

    [loop] while (true) {
        uint mask = 1U << (numLayers - curLayer - 1);
        double rad = (double)(1U << (numLayers - curLayer)) / 2.0;
        double3 mid = (double3)(path & uint3(~mask, ~mask, ~mask)) + double3(rad, rad, rad);

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

            double3 curPos = origin + ray * curScale;
            indices[curLayer] = nextIndex;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                if (curPos[i] >= mid[i])
                    path[i] |= mask;
                else
                    path[i] &= ~mask;
            }
        }
        else {
            uint axis = -1;
            double bestScale = 1.#INF;
            [unroll(3)] for (uint i = 0; i < 3; i++) {
                double s = (mid[i] - origin[i]) * invRay[i];
                if (s <= curScale) // Might cause errors because of double inaccuracy (?)
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
            result.collision[result.side] += 1.0f;
    }
    return result;
}


#endif