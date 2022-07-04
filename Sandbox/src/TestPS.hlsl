struct VSOutput {
    float4 pixelPosition : SV_POSITION;
    float3 cameraPosition : CAMERA_POSITION;
    float3 sightRay : SIGHT_RAY;
};

struct MaterialGroup {
    uint indices[2][2][2];
};

StructuredBuffer<MaterialGroup> tree : register(t0);

float4 main(VSOutput input) : SV_TARGET
{
    const double3 origin = input.cameraPosition;
    const double3 ray = input.sightRay;
    const double3 invRay = 1.0f / ray;
    
    const uint numLayers = 24;

    double curScale = 0.0f;

    { // Trace to the root cube
        double rad = (float)(1 << numLayers) / 2.0f;
        double3  mid = double3 (rad, rad, rad) - origin;
        double3  minDiv = mid * invRay - abs(invRay * rad);
        double3  maxDiv = mid * invRay + abs(invRay * rad);
        
        double minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0f);
        double maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);
        
        if (minScale > maxScale) {
            return 0.0f;
        }
        curScale = minScale;
    }

    uint3 path = 0;
    uint curLayer = 0;
    uint indices[numLayers];
    uint nextIndex = 0;
    bool down = true;

    uint maxLayer = 0;
    [loop] while (true) {
        if (curLayer > maxLayer)
            maxLayer = curLayer;
        uint mask = 1U << (numLayers - curLayer - 1);
        double rad = (float)(1U << (numLayers - curLayer)) / 2.0f;
        double3 mid = (double3)(path & uint3(~mask, ~mask, ~mask)) + double3 (rad, rad, rad);

        if (down) {
            if (curLayer == numLayers) {
                float f = (float)nextIndex / 1000000.0f;
                return float4(0.0f, f, f, 1.0f);
            }

            //if (curLayer == 0) {
                //double3  minDiv = (mid - origin) * invRay - abs(invRay * rad);
                //double3  maxDiv = (mid - origin) * invRay + abs(invRay * rad);
                //
                //double minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0f);
                //double maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);
                //
                //if (minScale > maxScale) {
                //    if (curLayer == 0)
                //        break;
                //    curLayer--;
                //    continue;
                //}
                //curScale = minScale;
            //}

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
            //return float4(0.0f, 0.0f, 1.0f, 1.0f);
            //if (curLayer != numLayers - 1)
            //    return float4(1.0f, 1.0f, 0.0f, 1.0f);

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

            //if (axis == 0)
            //    return float4(1.0f, 0.0f, 0.0f, 1.0f);
            //else if (axis == 1)
            //    return float4(0.0f, 1.0f, 0.0f, 1.0f);
            //else
            //    return float4(0.0f, 0.0f, 1.0f, 1.0f);

            // CHECK IF INSIDE !!
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
            curScale = bestScale;
            if (axis == 0)
                path.x ^= mask;
            else if (axis == 1)
                path.y ^= mask;
            else
                path.z ^= mask;
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
    //float f = (float)maxLayer / (float)numLayers;
    //return float4(f, f, f, 1.0f);
    return 0.0f;//float4(0.0f, 0.0f, 1.0f, 1.0f);
}

    //const float3 origin = input.cameraPosition;
    //const float3 ray = input.sightRay;
    //const float3 invRay = 1.0f / ray;
    //
    //const uint numLayers = 8;
    //
    //uint3 path = uint3(0, 0, 0);
    //
    //uint indices[numLayers];
    //indices[0] = 0;
    //
    //uint groups[numLayers];
    //groups[0] = 0;
    //
    //uint curLayer = 0;
    //
    //bool ret = false;
    //
    //uint cnt = 0;
    //
    //uint i = 0;
    //[loop] while (true) {
    //    if (indices[curLayer] == 1)
    //        return float4(1.0f, 1.0f, 0.0f, 1.0f);
    //    indices[curLayer + 1] = 1;// tree[0].indices[0][0][0];
    //    curLayer++;
    //    if (++cnt == 5)
    //        break;
    //    //uint mask = 1U << (numLayers - curLayer - 1);
    //    //if (groups[curLayer] == 0) {
    //    //    float rad = (float)mask / 2.0f;
    //    //    float3 mid = (float3)path + float3(rad, rad, rad) - origin;
    //    //
    //    //    float3 minDiv = mid * invRay - abs(invRay * rad);
    //    //    float3 maxDiv = mid * invRay + abs(invRay * rad);
    //    //
    //    //    float minScale = max(max(minDiv.x, minDiv.y), minDiv.z);
    //    //    float maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);
    //    //
    //    //    if (minScale > maxScale || maxScale < 0.0f) {
    //    //        groups[curLayer] = (1 << 3);
    //    //    }
    //    //}
    //    //if (groups[curLayer] == (1 << 3)) {
    //    //    if (curLayer == 0)
    //    //        break;
    //    //    path.x &= ~mask;
    //    //    path.y &= ~mask;
    //    //    path.z &= ~mask;
    //    //    curLayer--;
    //    //    continue;
    //    //}
    //    //
    //    //uint ind = tree[indices[curLayer]].indices[(bool)(groups[curLayer] & (1 << 2))]
    //    //    [(bool)(groups[curLayer] & (1 << 1))]
    //    //    [(bool)(groups[curLayer] & (1 << 0))];
    //    //if (ind == -1) {
    //    //    groups[curLayer]++;
    //    //    continue;
    //    //}
    //    //if (groups[curLayer] & (1 << 2))
    //    //    path.x |= mask;
    //    //else
    //    //    path.x &= ~mask;
    //    //if (groups[curLayer] & (1 << 1))
    //    //    path.y |= mask;
    //    //else
    //    //    path.y &= ~mask;
    //    //if (groups[curLayer] & (1 << 0))
    //    //    path.z |= mask;
    //    //else
    //    //    path.z &= ~mask;
    //    //groups[curLayer]++;
    //    //
    //    //if (curLayer == numLayers - 1) {
    //    //    if (ind == 1 && indices[curLayer] == 1 && indices[curLayer - 1] == 2)
    //    //        return float4(0.0f, 1.0f, 1.0f, 1.0f);
    //    //    else
    //    //        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //    //}
    //    //curLayer++;
    //    //indices[curLayer] = ind;
    //    //groups[curLayer] = 0;
    //
    //}
    //
    //return float4(0.0f, 0.0f, 0.0f, 1.0f);

    ////const float3 origin = input.cameraPosition;
    ////const float3 ray = input.sightRay;
    ////const float3 invRay = 1.0f / ray;
    ////
    ////
    //
    //const uint numLayers = 8;
    //
    //const float3 origin = input.cameraPosition;
    //const float3 ray = input.sightRay;
    //const float3 invRay = 1.0f / ray;
    //
    //uint indices[numLayers];
    //indices[0] = 0;
    //bool3 checked[numLayers];
    //checked[0] = false;
    //
    //uint3 curPath = 0;
    //
    //uint curLayer = 0;
    //bool down = true;
    //[loop] while (true) {
    //    uint mask = (1 << (numLayers - curLayer - 1));
    //    float rad = (float)mask / 2.0f;
    //    float3 mid = (float3)curPath + float3(rad, rad, rad) - origin;
    //    if (down) {
    //        float3 invRad = abs(invRay * rad);
    //        float3 minPos = mid * invRay - invRad;
    //        float3 maxPos = mid * invRay + invRad;
    //        float minScale = max(max(minPos.x, minPos.y), minPos.z);
    //        float maxScale = min(min(maxPos.x, maxPos.y), maxPos.z);
    //        minScale = max(minScale, 0.0f);
    //        if (minScale > maxScale) {
    //            down = false;
    //        }
    //        else {
    //            float3 offset = (ray * minScale) - mid;
    //            if (offset.x >= 0.0f)
    //                curPath.x |= mask;
    //            else
    //                curPath.x &= ~mask;
    //            if (offset.y >= 0.0f)
    //                curPath.y |= mask;
    //            else
    //                curPath.y &= ~mask;
    //            if (offset.z >= 0.0f)
    //                curPath.z |= mask;
    //            else
    //                curPath.z &= ~mask;
    //        }
    //    }
    //    else {
    //        uint bestInd = -1;
    //        float bestScale = 1.#INF;
    //        //float3 offset = mid - origin;
    //        for (uint i = 0; i < 3; i++) {
    //            if (ray[i] == 0.0f)
    //                continue;
    //            if (checked[curLayer][i])
    //                continue;
    //            float scale = mid[i] * invRay[i];
    //            if (scale < bestScale)
    //                bestInd = i;
    //        }
    //        if (bestInd == -1) {
    //            down = false;
    //        }
    //        else {
    //            if (bestInd == 0)
    //                checked[curLayer].x = true;
    //            else if (bestInd == 1)
    //                checked[curLayer].y = true;
    //            else
    //                checked[curLayer].z = true;
    //            if ((bool)(curPath[bestInd] & mask)) {
    //                if (bestInd == 0)
    //                    curPath.x &= ~mask;
    //                else if (bestInd == 1)
    //                    curPath.y &= ~mask;
    //                else
    //                    curPath.z &= ~mask;
    //            }
    //            else {
    //                if (bestInd == 0)
    //                    curPath.x |= mask;
    //                else if (bestInd == 1)
    //                    curPath.y |= mask;
    //                else
    //                    curPath.z |= mask;
    //            }
    //            down = true;
    //        }
    //    }
    //    if (!down) {
    //        if (curLayer == 0)
    //            break;
    //        curLayer--;
    //        continue;
    //    }
    //    uint ind = tree[indices[curLayer]].indices[(bool)(curPath.x & mask)]
    //        [(bool)(curPath.y & mask)]
    //        [(bool)(curPath.z & mask)];
    //    if (ind == -1) {
    //        down = false;
    //        continue;
    //    }
    //    if (curLayer == numLayers - 1) {
    //        return float4(0.0f, 1.0f, 1.0f, 1.0f);
    //    }
    //    else {
    //        curLayer++;
    //        indices[curLayer] = ind;
    //        checked[curLayer] = false;
    //    }
    //}
    //return 0.0f;
    ////return float4(1.0f, 0.0f, 0.0f, 1.0f);
//}