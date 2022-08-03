#ifndef PS_LIGHTTRACE
#define PS_LIGHTTRACE

#include "VoxelPS_input.hlsl"
#include "VoxelPS_voxeltrace.hlsl"
#include "VoxelPS_sky.hlsl"

void LightTrace(float3 origin, float3 startRay, float2 pixelPosition) {
	const uint numRays = scene[0].numRays;
	const float2 windowSize = (float2)scene[0].windowSize;
	const uint uMod = 10000;

	Ray outRay = rays[(uint)pixelPosition.y * scene[0].windowSize.x + (uint)pixelPosition.x];
	if (scene[0].newRays)
		outRay.numRays = 0;

	VoxelRay primary = VoxelTrace(origin, startRay, uint3(~0, ~0, ~0));
	//VoxelRay primary;
	////
	float3 totalLight = 0.0f;
	//float3 multiplier = float3(1.0f, 1.0f, 1.0f);
	//
	//VoxelRay current;
	//current.collision = origin;
	//current.voxel = uint3(~0, ~0, ~0);
	//
	//float3 ray = startRay;
	//
	//for (uint i = 0; i < numRays; i++) {
	//	if (i > 0) {
	//		float2 angle = float2(rand() * (4.0f * PI), rand() * (4.0f * PI));
	//		ray = float3(sin(angle.x) * cos(angle.y), abs(sin(angle.y)), cos(angle.x) * cos(angle.y));
	//		if (current.normal[current.side] < 0.0f)
	//			ray.y = -ray.y;
	//		if (current.side == 0)
	//			ray.xyz = ray.yxz;
	//		else if (current.side == 2)
	//			ray.xyz = ray.xzy;
	//	}
	//	current = VoxelTrace(current.collision, ray, current.voxel);
	//	if (i == 0)
	//		primary = current;
	//
	//	if (current.material == ~0) {
	//		totalLight += SkyTrace(ray) * multiplier;
	//		break;
	//	}
	//
	//	totalLight += materials[current.material].light * multiplier;
	//	if (i > 0)
	//		multiplier *= materials[current.material].color;
	//}

	if (primary.material == ~0){
		totalLight = SkyTrace(startRay);
	}
	else {
		for (uint i = 0; i < numRays; i++) {
			//float seed = (uint(pixelPosition.y * windowSize.x + pixelPosition.x + (float)i) + outRay.numRays) % uMod;
			//float seed = (float)(random[0]++ % uMod);
			//float3 ray = 0.0f;
			//ray.y = 0.1f + frac(PHI * seed) * 0.9f;
			//float rad = sqrt(1.0f - (ray.y * ray.y));
			//ray.x = cos(angle) * rad;
			//ray.z = sin(angle) * rad;
	
			float2 angle = float2(randFloat((uint2)pixelPosition, outRay.numRays + i) * (2.0f * PI), randFloat((uint2)pixelPosition, outRay.numRays + i + 1) * (2.0f * PI));
			float3 ray = float3(sin(angle.x) * cos(angle.y), abs(sin(angle.y)), cos(angle.x) * cos(angle.y));
	
			if (primary.normal[primary.side] < 0.0f)
				ray.y = -ray.y;
	
			if (primary.side == 0)
				ray.xyz = ray.yxz;
			else if (primary.side == 2)
				ray.xyz = ray.xzy;
			VoxelRay secondaryRay = VoxelTrace(primary.collision, ray, primary.voxel);
			if (secondaryRay.material == ~0)
				totalLight += SkyTrace(ray);// * materials[primary.material].color.rgb;
			else
				totalLight += materials[secondaryRay.material].light;// * materials[primary.material].color.rgb;
	
			//if (primary.side == 0)
			//	ray.xyz = ray.yxz;
			//else if (primary.side == 2)
			//	ray.xyz = ray.xzy;
			//VoxelRay secondaryRay = VoxelTrace(primary.collision, ray, primary.voxel);
			//if (secondaryRay.material != ~0)
			//	totalLight += float3(1.0f, 1.0f, 1.0f);
		}
		totalLight /= (float)numRays;
		totalLight += materials[primary.material].light;
	}

	outRay.voxel = primary.voxel;
	outRay.material = primary.material;
	outRay.side = primary.side;
	outRay.numRays++;
	outRay.light = (1.0f - 1.0f / (float)outRay.numRays) * outRay.light + (1.0f / (float)outRay.numRays) * totalLight;
	//ray.light = totalLight;

	rays[(uint)pixelPosition.y * scene[0].windowSize.x + (uint)pixelPosition.x] = outRay;
	//return totalLight;
}

#endif