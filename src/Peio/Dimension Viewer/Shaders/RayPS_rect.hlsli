#ifndef PS_RECT
#define PS_RECT

#include "RayPS_input.hlsli"

struct HyperRect {
    float position[numDims];
    float size[numDims];
	Material material;
};
StructuredBuffer<HyperRect> hyperRects : register(t3);

bool TraceRect(float origin[numDims], float ray[numDims], in out float scale, out uint outRect, out float normal[numDims]){
    const uint numRects = scene[0].numRects;
    
    float scale = 1.#INF;
    uint side = 0;
    bool hit = false;
    
    [loop] for (uint i = 0; i < numRects; i++){
        HyperRect rect = hyperRects[i];
		
		float sMin = -1.#INF;
		float sMax = 1.#INF;
        uint curSideMin = 0, curSideMax = 0;
		
		for (uint j = 0; j < numDims; j++){
			float first = (rect.position[j] - origin[j]) / ray[j];
			float second = (rect.position[j] + rect.size[j] - origin[j]) / ray[j];
			if (min(first, second) > sMin){
				sMin = min(first, second);
                curSideMin = j;
            }
			if (max(first, second) < sMax){
				sMax = max(first, second);
                curSideMax = j;
            }
		}
        
        if (sMin > sMax || sMax <= LIM)
			continue;
            
        float s = sMin;
        if (s <= LIM)
            s = sMax;
        
        if (s < scale){
            scale = s;
            if (s == sMin)
                side = curSideMin;
            else
                side = curSideMax;
            outRect = i;
            hit = true;
        }
    }
    if (hit){
        [unroll] for (uint i = 0; i < numDims; i++)
            normal[i] = 0.0f;
        normal[side] = 1.0f;
    }
    return hit;
}

#endif