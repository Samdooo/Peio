#ifndef PS_LIGHT
#define PS_LIGHT

#include "RayPS_trace.hlsli"
#include "RayPS_random.hlsli"
#include "Ray_math.hlsli"

float3 BackLight(Vector ray){
    return scene[0].backLight;
}

void Light(Vector origin, Vector ray, uint2 pixelPosition){
    const uint numBounces = scene[0].numBounces;
    
    Normalize(ray);
    
    uint diffuseCount[maxBounces];
    float3 light[maxBounces];
    float cosines[maxBounces];
    float cosineSums[maxBounces];
    Collision cols[maxBounces];
    
    cols[0] = Trace(origin, ray);
    
    bool up = true;
    uint curBounce = 0;
    uint randAdd = 0;
    
    NoiseRay noiseRay;
    noiseRay.scale = 1.#INF;
    [unroll] for (uint i = 0; i < numDims; i++)
        noiseRay.normal.v[i] = 0.0f;
    noiseRay.diffuseLight = 0.0f;
    noiseRay.reflection = 0.0f;
    noiseRay.emission = 0.0f;
    noiseRay.spectral = 0.0f;
    
    [loop] while (true) {
        if (up && cols[curBounce].scale == 1.#INF){
            light[curBounce] = BackLight(cols[curBounce].ray);
            up = false;
            if (curBounce == 0){
                noiseRay.emission = light[curBounce];
            }
        }
        else {
            if (curBounce == numBounces - 1){
                light[curBounce] = cols[curBounce].material.emission;
                up = false;
            }
            else {
                if (up){
                    diffuseCount[curBounce] = 0;
                    light[curBounce] = 0.0f;
                    cosineSums[curBounce] = 0.0f;
                }
                if (diffuseCount[curBounce] > 0)
                    light[curBounce] += light[curBounce + 1] * cosines[curBounce];
                
                if (diffuseCount[curBounce] == raysPerBounce[curBounce]){
                    if (curBounce == 0){
                        noiseRay.emission = (light[curBounce] - noiseRay.diffuseLight) * cols[0].material.reflection + cols[curBounce].material.emission;
                    }
                    light[curBounce] *= cols[curBounce].material.reflection;
                    light[curBounce] += cols[curBounce].material.emission;
                    up = false;
                }
                else if (diffuseCount[curBounce] == raysPerBounce[curBounce] - 1){
                    if (cosineSums[curBounce] > 0.0f)
                        light[curBounce] *= (1.0f - cols[curBounce].material.spectral) / cosineSums[curBounce];
                    else 
                        light[curBounce] = 0.0f;
                    
                    if (curBounce == 0){
                        noiseRay.diffuseLight = light[curBounce];
                    }
                        
                    cosines[curBounce] = cols[curBounce].material.spectral;
                    float dot = Dot(cols[curBounce].ray, cols[curBounce].normal);
                    Vector spectralRay = cols[curBounce].ray;
                    [unroll] for (uint i = 0; i < numDims; i++)
                        spectralRay.v[i] -= cols[curBounce].normal.v[i] * 2.0f * dot;
                    cols[curBounce + 1] = Trace(cols[curBounce].position, spectralRay);
                    diffuseCount[curBounce]++;
                    up = true;
                }
                else {
                    Vector diffuseRay = RandVector(pixelPosition, randAdd++);
                    if (Dot(diffuseRay, cols[curBounce].normal) < 0.0f){
                        [unroll] for (uint i = 0; i < numDims; i++)
                            diffuseRay.v[i] = -diffuseRay.v[i];
                    }
                    cosines[curBounce] = Cosine(diffuseRay, cols[curBounce].normal);
                    cosineSums[curBounce] += cosines[curBounce];
                    cols[curBounce + 1] = Trace(cols[curBounce].position, diffuseRay);
                    diffuseCount[curBounce]++;
                    up = true;
                }
            }
        }
        
        if (up){
            curBounce++;
        }
        else {
            if (curBounce == 0)
                break;
            curBounce--;
        }
    }
    //return light[0];
    
    noiseRay.scale = cols[0].scale;
    if (noiseRay.scale != 1.#INF){
        noiseRay.normal = cols[0].normal;
        noiseRay.reflection = cols[0].material.reflection;
        noiseRay.spectral = cols[0].material.spectral;
    }
    noiseRays[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x] = noiseRay;
}

#endif