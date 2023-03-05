#ifndef RAY_HEADER
#define RAY_HEADER

//#define numDims 4

static const float limit = 1.0f / (float)(1U << 12); // Used instead of zero in some cases to make up for floating point inaccuracy

struct VSOutput {
    float4 position : SV_POSITION;
	float sightRay[numDims] : SIGHT_RAY;
};

#endif