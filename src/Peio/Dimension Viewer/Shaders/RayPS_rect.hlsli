#ifndef PS_RECT
#define PS_RECT

#include "Ray_input.hlsli"

struct HyperRect {
    Vector position;
    Vector size;
	Material material;
};
StructuredBuffer<HyperRect> hyperRects : register(t3);

bool InsideRect(uint ind, Vector position){
    HyperRect rect = hyperRects[ind];
    [unroll] for (uint i = 0; i < numDims; i++)
        if (position.v[i] < rect.position.v[i] || position.v[i] > (rect.position.v[i] + rect.size.v[i]))
            return false;
    return true;
}

bool TraceRect(Vector origin, Vector ray, in out float scale, out uint outRect, out Vector normal){
    const uint numRects = scene[0].numRects;
    
    uint side = 0;
    bool sign = 0;
    bool hit = false;
    
    [loop] for (uint i = 0; i < numRects; i++){
        HyperRect rect = hyperRects[i];
		
		float sMin = -1.#INF;
		float sMax = 1.#INF;
        uint curSideMin = 0, curSideMax = 0;
        bool curSignMin = false, curSignMax = false;
		
		for (uint j = 0; j < numDims; j++){
			float first = (rect.position.v[j] - origin.v[j]) / ray.v[j];
			float second = (rect.position.v[j] + rect.size.v[j] - origin.v[j]) / ray.v[j];
			if (min(first, second) > sMin){
				sMin = min(first, second);
                curSideMin = j;
                curSignMin = (first < second);
            }
			if (max(first, second) < sMax){
				sMax = max(first, second);
                curSideMax = j;
                curSignMax = (first > second);
            }
		}
        
        if (sMin > sMax || sMax <= LIM)
			continue;
            
        float s = sMin;
        if (s <= LIM)
            s = sMax;
        
        if (s < scale){
            scale = s;
            if (s == sMin){
                side = curSideMin;
                sign = curSignMin;
            }
            else {
                side = curSideMax;
                sign = curSignMax;
            }
            outRect = i;
            hit = true;
        }
    }
    if (hit){
        [unroll] for (uint i = 0; i < numDims; i++)
            normal.v[i] = 0.0f;
        if (sign)
            normal.v[side] = -1.0f;
        else
            normal.v[side] = 1.0f;
    }
    return hit;
}

#endif