#ifndef PS_TRACE
#define PS_TRACE

#include "RayPS_rect.hlsli"
#include "RayPS_sphere.hlsli"

struct Collision {
    float ray[numDims];
    float scale;
    float position[numDims];
    float normal[numDims];
    Material material;
};

Collision Trace(float origin[numDims], float ray[numDims]){
    Collision col;
    col.scale = 1.#INF;
    col.ray = ray;
    
    uint ind;
    bool hitRect = TraceRect(origin, ray, col.scale, ind, col.normal);
    bool hitSphere = TraceSphere(origin, ray, col.scale, ind, col.normal);
    
    if (hitSphere)
        col.material = hyperSpheres[ind].material;
    else if (hitRect)
        col.material = hyperRects[ind].material;
    
    return col;
}

#endif