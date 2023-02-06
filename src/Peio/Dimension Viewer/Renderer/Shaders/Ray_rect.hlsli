#ifndef RECT_HEADER
#define RECT_HEADER

#include "Ray_math.hlsli" 
#include "Ray_scene.hlsli"

struct Rect {
    float low[numDims];
    float high[numDims];
    Material material;
};
StructuredBuffer<Rect> rects : register(t2);

bool TraceRect(uint ind, in float origin[numDims], in float ray[numDims], in out float scale){
    const Rect rect = rects[ind];
    
    float sMin = -1.#INF, sMax = 1.#INF;
    [unroll(numDims)] for (uint i = 0; i < numDims; i++){
        float sda = (rect.low[i] - origin[i]) / ray[i];
        float sdb = (rect.high[i] - origin[i]) / ray[i];
        float sdMin = min(sda, sdb);
        float sdMax = max(sda, sdb);
        if (sdMin > sMin)
            sMin = sdMin;
        if (sdMax < sMax)
            sMax = sdMax;
    }
    if (sMax <= limit || sMax < sMin || sMin >= scale)
        return false;
    if (sMin > limit)
        scale = sMin;
    else
        scale = sMax;
    return true;
}

void NormalRect(uint ind, in float p[numDims], out float normal[numDims]){
    const Rect rect = rects[ind];
    [unroll(numDims)] for (uint i = 0; i < numDims; i++){
        if (abs(rect.low[i] - p[i]) <= limit)
            normal[i] = -1.0f;
        else if (abs(p[i] - rect.high[i]) <= limit)
            normal[i] = 1.0f;
        else
            normal[i] = 0.0f;
    }
}

#endif