#include "Ray_trace.hlsli"
#include "Ray_sky.hlsli"
#include "Ray_math.hlsli"

//#define maxBounces 2

#define LIGHT_STATE_NEW 0
#define LIGHT_STATE_FINISHED 1

void LightTrace(in float origin[numDims], in float ray[numDims], in out DenoisePixel result){
    Normalize(ray);
    
    uint rayCount[maxBounces];
    float rays[maxBounces][numDims];
    TracedRay traces[maxBounces];
    float normals[maxBounces][numDims];
    float3 lights[maxBounces];
    float cosines[maxBounces];
    float cosineSums[maxBounces];
    
    rays[0] = ray;
    TraceRay(origin, ray, uint2(~0, ~0), traces[0]);
    
    uint curBounce = 0;
    uint state = LIGHT_STATE_NEW;
    
    [loop] while (true){
        if (state == LIGHT_STATE_NEW){
            lights[curBounce] = 0.0f;
            if (traces[curBounce].obj[0] == ~0 || curBounce == maxBounces - 1){
                state = LIGHT_STATE_FINISHED;
            }
            else {
                rayCount[curBounce] = 0;
                cosineSums[curBounce] = 0.0f;
                TracedNormal(traces[curBounce], normals[curBounce]);
                Normalize(normals[curBounce]);
                GenerateDiffuse(rays[curBounce], normals[curBounce], rays[curBounce + 1]);
                Normalize(rays[curBounce + 1]);
                cosines[curBounce] = VectorCos(normals[curBounce], rays[curBounce + 1]);
            }
        }
        else if (state == LIGHT_STATE_FINISHED){
            rayCount[curBounce]++;
            
            if (rayCount[curBounce] < raysPerBounce[curBounce]){
                lights[curBounce] += lights[curBounce + 1] * cosines[curBounce];
                cosineSums[curBounce] += cosines[curBounce];
            }
            else {
                lights[curBounce] += lights[curBounce + 1] * GetMaterial(traces[curBounce].obj).specular;
            }
            
            if (rayCount[curBounce] < raysPerBounce[curBounce] - 1){
                GenerateDiffuse(rays[curBounce], normals[curBounce], rays[curBounce + 1]);
                Normalize(rays[curBounce + 1]);
                cosines[curBounce] = VectorCos(normals[curBounce], rays[curBounce + 1]);
                state = LIGHT_STATE_NEW;
            }
            else if (rayCount[curBounce] == raysPerBounce[curBounce] - 1){
                lights[curBounce] *= (1.0f - GetMaterial(traces[curBounce].obj).specular);
                if (cosineSums[curBounce] > 0.0f)
                    lights[curBounce] /= cosineSums[curBounce];
                Reflect(rays[curBounce], normals[curBounce], rays[curBounce + 1]);
                Normalize(rays[curBounce + 1]);
                state = LIGHT_STATE_NEW;
            }
            else {
                lights[curBounce] *= GetMaterial(traces[curBounce].obj).reflection;
                state = LIGHT_STATE_FINISHED;
            }
        }
        
        if (state == LIGHT_STATE_NEW){
            TraceRay(traces[curBounce].collision, rays[curBounce + 1], traces[curBounce].obj, traces[curBounce + 1]);
            curBounce++;
        }
        else if (state == LIGHT_STATE_FINISHED){
            if (traces[curBounce].obj[0] == ~0)
                lights[curBounce] += SkyTrace(rays[curBounce]);
            else
                lights[curBounce] += GetMaterial(traces[curBounce].obj).emission;
            if (curBounce == 0)
                break;
            else
                curBounce--;
        }
    }
    if (traces[0].obj[0] == ~0){
        result.position[0] = 1.#INF;
    }
    else {
        result.position = traces[0].collision;
        result.normal = normals[0];
    }
    result.light = lights[0];
}