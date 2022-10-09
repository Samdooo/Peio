#ifndef PS_RANDOM
#define PS_RANDOM

#include "Ray_input.hlsli"

StructuredBuffer<uint> seeds : register(t5);
StructuredBuffer<Vector> randVectors : register(t6);

Vector RandVector(uint2 pixelPosition, uint add){
    return randVectors[(seeds[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x] + add) % scene[0].numRands];
}

#endif