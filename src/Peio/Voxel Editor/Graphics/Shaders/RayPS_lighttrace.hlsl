#ifndef PS_LIGHTTRACE
#define PS_LIGHTTRACE

#include "RayPS_input.hlsl"
#include "RayPS_voxeltrace.hlsl"
#include "RayPS_sky.hlsl"

void LightTrace(float3 origin, float3 startRay, float2 fPixelPosition) {
	const uint numRays = scene[0].numRays;
	const uint2 pixelPosition = (uint2)fPixelPosition;

	Ray outRay = rays[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];
	if (scene[0].newRays)
		outRay.numRays = 0;

	float3 totalLight = 0.0f;

	VoxelRay primary = VoxelTrace(origin, startRay);
	outRay.material = primary.material;

	if (primary.material == ~0) {
		totalLight = SkyTrace(startRay);
	}
	else {
		outRay.voxel = primary.voxel[primary.side];
		outRay.side = primary.side;
		
		VoxelRay oldRay = primary;
		float3 mul = float3(1.0f, 1.0f, 1.0f);
		for (uint i = 0; i < numRays; i++) {
			float2 angle = float2(randFloat(pixelPosition, outRay.numRays + i) * (2.0f * PI), randFloat(pixelPosition, outRay.numRays + i + 1) * (2.0f * PI));
			float3 ray = float3(sin(angle.x) * cos(angle.y), abs(sin(angle.y)), cos(angle.x) * cos(angle.y));

			if (primary.normal[primary.side] < 0.0f)
				ray.y = -ray.y;

			if (primary.side == 0)
				ray.xyz = ray.yxz;
			else if (primary.side == 2)
				ray.xyz = ray.xzy;
			
			float3 nm = 0.0f;
			nm[primary.side] = (primary.normal[primary.side] < 0.0f) ? -1.0f : 1.0f;

			float cd = distance(nm, normalize(ray));
			float cosMul = 1.0f - cd * cd / 2.0f;
			//mul *= cosMul;

			VoxelRay newRay = VoxelTrace(oldRay.collision, ray);
			if (newRay.material == ~0){
				totalLight += SkyTrace(ray) * mul;
				break;
			}
			totalLight += materials[newRay.material].light * mul;
			mul *= materials[newRay.material].color.rgb;
			oldRay = newRay;
		}
	}

	outRay.numRays++;
	outRay.light = (1.0f - 1.0f / (float)outRay.numRays) * outRay.light + (1.0f / (float)outRay.numRays) * totalLight;

	rays[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x] = outRay;
}

//void LightTrace(float3 origin, float3 startRay, float2 pixelPosition) {
//	const uint numRays = scene[0].numRays;
//	const float2 windowSize = (float2)scene[0].windowSize;
//
//	Ray outRay = rays[(uint)pixelPosition.y * scene[0].windowSize.x + (uint)pixelPosition.x];
//	if (scene[0].newRays)
//		outRay.numRays = 0;
//
//	VoxelRay primary = VoxelTrace(origin, startRay, uint3(~0, ~0, ~0));
//	float3 totalLight = 0.0f;
//
//	if (primary.material == ~0) {
//		totalLight = SkyTrace(startRay);
//	}
//	else {
//		for (uint i = 0; i < numRays; i++) {
//			float2 angle = float2(randFloat((uint2)pixelPosition, outRay.numRays + i) * (2.0f * PI), randFloat((uint2)pixelPosition, outRay.numRays + i + 1) * (2.0f * PI));
//			float3 ray = float3(sin(angle.x) * cos(angle.y), abs(sin(angle.y)), cos(angle.x) * cos(angle.y));
//
//			if (primary.normal[primary.side] < 0.0f)
//				ray.y = -ray.y;
//
//			if (primary.side == 0)
//				ray.xyz = ray.yxz;
//			else if (primary.side == 2)
//				ray.xyz = ray.xzy;
//
//			VoxelRay secondaryRay = VoxelTrace(primary.collision, ray, primary.voxel);
//			if (secondaryRay.material == ~0)
//				totalLight += SkyTrace(ray);
//			else
//				totalLight += materials[secondaryRay.material].light;
//		}
//		totalLight /= (float)numRays;
//		totalLight += materials[primary.material].light;
//	}
//
//	//outRay.voxel = primary.voxel;
//	if (primary.material != ~0) {
//		outRay.voxel = primary.voxel[primary.side];
//		outRay.material = primary.material;
//		outRay.side = primary.side;
//	}
//	else {
//		outRay.material = ~0;
//	}
//
//	outRay.numRays++;
//	outRay.light = (1.0f - 1.0f / (float)outRay.numRays) * outRay.light + (1.0f / (float)outRay.numRays) * totalLight;
//
//	//if (outRay.numRays == 1)
//	//	outRay.light = 0.0f;
//	//else if (outRay.numRays == 70)
//	//	outRay.light = float3(0.0f, 0.0f, 1.0f)
//
//	rays[(uint)pixelPosition.y * scene[0].windowSize.x + (uint)pixelPosition.x] = outRay;
//}

#endif