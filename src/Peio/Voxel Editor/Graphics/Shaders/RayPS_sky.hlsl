#ifndef PS_SKY
#define PS_SKY

#include "RayPS_input.hlsl"

float3 RotateX(float3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return float3(p.x, c * p.y + s * p.z, -s * p.y + c * p.z);
}

float3 RotateY(float3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return float3(c * p.x - s * p.z, p.y, s * p.x + c * p.z);
}

float3 SkyTrace(float3 ray) {
    float3 sunRay = float3(0.0f, 0.0f, 1.0f);
    sunRay = RotateX(sunRay, scene[0].sky.sunRotation.y);
    sunRay = RotateY(sunRay, scene[0].sky.sunRotation.x);

    ray = normalize(ray);

    float3 sky_day = float3(0.5f, 0.8f, 0.9f);
    float3 sky_night = float3(0.06f, 0.02f, 0.25f);
    float3 sun = float3(20.0f, 20.0f, 10.0f) * 5.0f;

    float sun_size = 0.2f;

    float3 diff = length(ray - sunRay);
    float sunMul = (sunRay.y + 1.0f) / 2.0f;
    return (sky_day * sunMul + sky_night * (1.0f - sunMul));// +sun * max((sun_size - diff), 0.0f);
}

#endif