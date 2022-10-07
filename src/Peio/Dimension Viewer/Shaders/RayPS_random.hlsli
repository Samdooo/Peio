#ifndef PS_RANDOM
#define PS_RANDOM

#include "RayPS_input.hlsli"

StructuredBuffer<uint> seeds : register(t5);

struct Vector {
    float v[numDims];
};
StructuredBuffer<Vector> randVectors : register(t6);

Vector RandVector(uint2 pixelPosition, uint add){
    return randVectors[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x + add];
}

#endif