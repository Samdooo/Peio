#ifndef PS_LIGHTTRACE
#define PS_LIGHTTRACE

#include "VoxelPS_input.hlsl"
#include "VoxelPS_voxeltrace.hlsl"
#include "VoxelPS_sky.hlsl"

void LightTrace(float3 origin, float3 startRay, float2 pixelPosition) {
	const uint numRays = scene[0].numRays;
	const float2 windowSize = (float2)scene[0].windowSize;
	const uint uMod = 1000;

	VoxelRay primary = VoxelTrace(origin, startRay, uint3(~0, ~0, ~0));

	float3 totalLight = 0.0f;

	if (primary.material == ~0){
		totalLight = SkyTrace(startRay);
	}
	else {
		for (uint i = 0; i < numRays; i++) {
			float seed = uint(pixelPosition.y * windowSize.x + pixelPosition.x + (float)i) % uMod;
			float3 ray = 0.0f;
			ray.y = frac(PHI * seed);
			if (primary.normal[primary.side] < 0.0f)
				ray.y = -ray.y;
			float angle = GOLDEN_ANGLE * seed * 2;
			float rad = sqrt(1.0f - (ray.y * ray.y));
			ray.x = cos(angle) * rad;
			ray.z = sin(angle) * rad;
			if (primary.side == 0)
				ray.xyz = ray.yxz;
			else if (primary.side == 2)
				ray.xyz = ray.xzy;
			VoxelRay secondaryRay = VoxelTrace(primary.collision, ray, primary.voxel);
			if (secondaryRay.material == ~0)
				totalLight += SkyTrace(ray);// * materials[primary.material].color.rgb;
			else
				totalLight += materials[secondaryRay.material].light;// * materials[primary.material].color.rgb;
		}
		totalLight /= (float)numRays;
		totalLight += materials[primary.material].light;
	}

	Ray ray;
	ray.voxel = primary.voxel;
	ray.material = primary.material;
	ray.side = primary.side;
	ray.light = totalLight;

	rays[(uint)pixelPosition.y * scene[0].windowSize.x + (uint)pixelPosition.x] = ray;
	//return totalLight;
}

#endif