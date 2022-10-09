#ifndef PS_SPHERE
#define PS_SPHERE

#include "Ray_input.hlsli"

struct HyperSphere {
	Vector center;
	float radius;
	Material material;
};
StructuredBuffer<HyperSphere> hyperSpheres : register(t4);

bool InsideSphere(uint ind, Vector position){
    HyperSphere sphere = hyperSpheres[ind];
    float dist = 0.0f;
	[unroll] for (uint i = 0; i < numDims; i++)
		dist += (position.v[i] - sphere.center.v[i]) * (position.v[i] - sphere.center.v[i]);
	dist = sqrt(dist);
    return dist <= sphere.radius;
}

bool TraceSphere(Vector origin, Vector ray, in out float scale, in out uint outSphere, out Vector normal){
    const uint numSpheres = scene[0].numSpheres;
    
    bool hit = false;
    [loop] for (uint i = 0; i < numSpheres; i++){
        HyperSphere sphere = hyperSpheres[i];
        
        float a = 0.0f, b = 0.0f, c = 0.0f;
		for (uint j = 0; j < numDims; j++){
			a += ray.v[j] * ray.v[j];
			b += 2.0f * (origin.v[j] * ray.v[j] - sphere.center.v[j] * ray.v[j]);
			c += (origin.v[j] - sphere.center.v[j]) * (origin.v[j] - sphere.center.v[j]) - sphere.radius * sphere.radius;
		}
		float sqr = b * b - 4.0f * a * c;
		if (sqr < 0.0f)
			continue;
		
		float first = -b / (2.0f * a);
		float second = abs(sqrt(sqr) / (2.0f * a));
		
		float sMin = first - second;
		float sMax = first + second;
		
		if (sMax <= LIM)
			continue;
            
        float s = sMin;
		if (sMin <= LIM)
			s = sMax;
        if (s < scale){
			scale = s;
            outSphere = i;
            hit = true;
		}
    }
    if (hit){
        [unroll] for (uint i = 0; i < numDims; i++)
            normal.v[i] = (origin.v[i] + ray.v[i] * scale) - hyperSpheres[outSphere].center.v[i];
    }
    return hit;
}

#endif