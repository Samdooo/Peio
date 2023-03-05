#include "Ray_rect.hlsli"
#include "Ray_sphere.hlsli"

void TraceRay(in float origin[numDims], in float ray[numDims], uint2 skip, in out TracedRay result) {
    //float invRay[numDims];
    //[unroll(numDims)] for (uint i = 0; i < numDims; i++)
    //    invRay[i] = 1.0f / ray[i];
    
    result.obj[0] = ~0;
    float scale = 1.#INF;
    {
        [loop] for (uint i = 0; i < scene[0].numRects; i++){
            bool hit = TraceRect(i, origin, ray, scale);
            if (hit){
                uint2 obj = uint2(0, i);
                //if (skip[0] == obj[0] && skip[1] == obj[1])
                //    continue;
                result.obj = obj;
            }
        }
    }
    {
        [loop] for (uint i = 0; i < scene[0].numSpheres; i++){
            bool hit = TraceSphere(i, origin, ray, scale);
            if (hit) {
                uint2 obj = uint2(1, i);
                //if (skip[0] == obj[0] && skip[1] == obj[1])
                //    continue;
                result.obj = obj;
            }
        }
    }
    if (result.obj[0] != ~0){
        [unroll(numDims)] for (uint i = 0; i < numDims; i++){
            result.collision[i] = origin[i] + ray[i] * scale;
        }
    }
}

Material GetMaterial(uint2 obj){
    if (obj[0] == 0)
        return rects[obj[1]].material;
    else
        return spheres[obj[1]].material;
}

void TracedNormal(in TracedRay traced, out float normal[numDims]){
    if (traced.obj[0] == 0)
        NormalRect(traced.obj[1], traced.collision, normal);
    else
        NormalSphere(traced.obj[1], traced.collision, normal);
}