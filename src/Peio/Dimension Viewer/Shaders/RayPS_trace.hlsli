#ifndef PS_TRACE
#define PS_TRACE

#include "RayPS_rect.hlsli"
#include "RayPS_sphere.hlsli"
#include "Ray_math.hlsli"

struct Collision {
    Vector ray;
    float scale;
    Vector position;
    Vector normal;
    Material material;
};

Collision Trace(Vector origin, Vector ray){
    Collision col;
    col.scale = 1.#INF;
    col.ray = ray;
    
    uint ind;
    bool hitRect = TraceRect(origin, ray, col.scale, ind, col.normal);
    bool hitSphere = TraceSphere(origin, ray, col.scale, ind, col.normal);
    
    if (col.scale != 1.#INF){
        Normalize(col.normal);
        [unroll] for (uint i = 0; i < numDims; i++)
            col.position.v[i] = origin.v[i] + ray.v[i] * col.scale;
    }
    if (hitSphere){
        col.material = hyperSpheres[ind].material;
        if (InsideSphere(ind, origin)){
            [unroll] for (uint i = 0; i < numDims; i++)
                col.normal.v[i] = -col.normal.v[i];
        }
    }
    else if (hitRect){
        col.material = hyperRects[ind].material;
        if (InsideRect(ind, origin)){
            [unroll] for (uint i = 0; i < numDims; i++)
                col.normal.v[i] = -col.normal.v[i];
        }
    }
    
    return col;
}

#endif