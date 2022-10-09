#ifndef S_VECTOR
#define S_VECTOR

#include "Ray_input.hlsli"

float Dot(Vector a, Vector b){
    float dot = 0.0f;
    [unroll] for (uint i = 0; i < numDims; i++)
        dot += a.v[i] * b.v[i];
    return dot;
}

float Length(Vector a){
    float length = 0.0f;
    [unroll] for (uint i = 0; i < numDims; i++)
        length += a.v[i] * a.v[i];
    return sqrt(length);
}

void Normalize(in out Vector a){
    float m = 1.0f / Length(a);
    [unroll] for (uint i = 0; i < numDims; i++)
        a.v[i] *= m;
}

float Sine(Vector a, Vector b){
    Normalize(a);
    Normalize(b);
    float length = 0.0f;
    [unroll] for (uint i = 0; i < numDims; i++)
        length += (a.v[i] - b.v[i]) * (a.v[i] - b.v[i]);
    length = sqrt(length);
    return length * sqrt(1.0f - (length * 0.5f) * (length * 0.5f));
}

float Cosine(Vector a, Vector b){
    float sine = Sine(a, b);
    return sqrt(1.0f - (sine * sine));
}

#endif