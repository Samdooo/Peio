#ifndef MATH_HEADER
#define MATH_HEADER

#include "Ray_header.hlsli"
#include "Ray_scene.hlsli"

float Dot(float a[numDims], float b[numDims]){
    float result = 0.0f;
    [unroll(numDims)] for (uint i = 0; i < numDims; i++)
        result += a[i] * b[i];
    return result;
}

void Rotate2D2(in out float p[numDims], uint k, in float v){
    const float c = cos(v);
    const float s = sin(v);
    float x = p[1] * c - p[k + 1] * s;
    float y = p[k + 1] * c + p[1] * s;
    p[1] = x;
    p[k + 1] = y;
}

void Rotate2D(in out float p[numDims], uint k, in float v){
    const float c = cos(v);
    const float s = sin(v);
    float x = p[0] * c - p[k + 1] * s;
    float y = p[k + 1] * c + p[0] * s;
    p[0] = x;
    p[k + 1] = y;
}

void Rotate(in out float p[numDims], in float v[numDims - 1]){
    [unroll(numDims - 1)] for (uint i = 0; i < numDims - 1; i++)
        Rotate2D(p, i, v[i]);
}

void InvRotate(in out float p[numDims], in float v[numDims - 1]) {
    [unroll(numDims - 1)] for (uint i = 0; i < numDims - 1; i++)
        Rotate2D(p, numDims - 2 - i, -v[numDims - 2 - i]);
}

float GetLength(in out float p[numDims]){
    float length = 0.0f;
    [unroll(numDims)] for (uint i = 0; i < numDims; i++)
        length += p[i] * p[i];
    return sqrt(length);
}

void Normalize(in out float v[numDims]){
    float invLength = 1.0f / GetLength(v);
    [unroll(numDims)] for (uint i = 0; i < numDims; i++)
        v[i] *= invLength;
}

float VectorCos(float a[numDims], float b[numDims]){
    float d[numDims];
    [unroll(numDims)] for (uint i = 0; i < numDims; i++)
        d[i] = a[i] - b[i];
    float l = GetLength(d);
    return 1.0f - (l * l * 0.5f);
}

float VectorSin(float a[numDims], float b[numDims]){
    float c = VectorCos(a, b);
    return sqrt(1.0f - c * c);
}

void Reflect(float ray[numDims], float normal[numDims], out float reflected[numDims]){
    float dot = Dot(ray, normal);
    [unroll(numDims)] for (uint i = 0; i < numDims; i++){
        reflected[i] = ray[i] - normal[i] * 2.0f * dot;
    }
}

static uint seedAdd = 0;
void GenerateDiffuse(in float ray[numDims], in float normal[numDims], in out float diffuse[numDims]){
    seedAdd++;
    diffuse = randomRays[(pixels[pixInd].seed + seedAdd) % scene[0].numRandom].ray;
    if (sign(Dot(normal, diffuse)) == sign(Dot(normal, ray))){
        [unroll(numDims)] for (uint i = 0; i < numDims; i++){
            diffuse[i] = -diffuse[i];
        }
    }
}

#endif