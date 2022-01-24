#ifndef PS_LIGHTSPREAD
#define PS_LIGHTSPREAD

#include "VoxelPS_input.hlsl"

//struct LightSpread {
//    
//    uint numRays;
//    uint rayIndex;
//    uint childrenRays;
//    float lightSpread;
//    float3 normal;
//    float3 ray;
//    int side;
//    float3 tmp;
//    
//    bool Increment(){
//        if (rayIndex >= numRays)
//            return true;
//        if (lightSpread == 0.0f){
//            ray = normal;
//            rayIndex++;
//            return false;
//        }
//        tmp = 0.0f;
//        
//        tmp.y = 1.0f - (float)(rayIndex + 1) / (float)(numRays);
//        //tmp.y *= tmp.y;
//        float radius = sqrt(1.0f - tmp.y * tmp.y);
//        if (normal[side] < 0.0f)
//            tmp.y = -tmp.y;
//        
//        tmp.x = cos(GOLDEN_ANGLE * rayIndex) * radius;
//        tmp.z = sin(GOLDEN_ANGLE * rayIndex) * radius;
//        
//        switch (side){
//        case 0:
//            ray.xyz = tmp.yxz;
//            break;
//        case 1:
//            ray.xyz = tmp.xyz;
//            break;
//        case 2:
//            ray.xyz = tmp.xzy;
//            break;
//        default:
//            break;
//        }
//        ray = normal * (1.0f - lightSpread) + ray * lightSpread;
//        rayIndex++;
//        return false;
//    }
//    
//    void Reset(float3 normal, float lightSpread, int side, uint maxRays, uint maxHitRays){
//        this.normal = normal;
//        this.lightSpread = lightSpread;
//        this.side = side;
//        
//        //numRays = maxRays;
//        numRays = min(maxRays, (uint)(lightSpread * (float)(maxHitRays - 1)) + 1);
//        //numRays = 2;
//        childrenRays = (maxRays - numRays) / numRays;
//        rayIndex = 0;
//        Increment();
//    }
//    
//};

struct LightSpread {
    
    uint numRays;
    uint rayIndex;
    uint childrenRays;
    float spread;
    float3 normal;
    float3 ray;
    int side;
    //float3 tmp;
    //float radius;
    
    bool Increment() {
        if (rayIndex == numRays)
            return true;
        if (spread == 0.0f){
            ray = normal;
            rayIndex++;
            return false;
        }
        
        float radius = (float)rayIndex / (float)(numRays - 1);
        
        float3 tmp;
        tmp.y = 1.0f + scene[0].minLightDeviation - radius;
        if (normal[side] < 0.0f)
            tmp.y = -tmp.y;
        
        tmp.x = cos(GOLDEN_ANGLE * rayIndex) * radius;
        tmp.z = sin(GOLDEN_ANGLE * rayIndex) * radius;
        
        switch (side){
        case 0:
            ray.xyz = tmp.yxz;
            break;
        case 1:
            ray.xyz = tmp.xyz;
            break;
        case 2:
            ray.xyz = tmp.xzy;
            break;
        default:
            break;
        }
        ray = (spread * ray) + ((1.0f - spread) * normal);
        rayIndex++;
        return false;
    }
    
    void Reset(float3 normal, float spread, int side, uint maxRays, uint maxHitRays) {
        this.normal = normalize(normal);
        this.spread = spread;
        this.side = side;
        
        numRays = min(maxRays, spread * (float)(maxHitRays - 1) + 1);
        childrenRays = (maxRays - numRays) / numRays;
        rayIndex = 0;
        Increment();
    }
};

#endif