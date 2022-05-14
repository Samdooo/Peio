#ifndef PS_LIGHTTRACE
#define PS_LIGHTTRACE

#include "VoxelPS_input.hlsl"
#include "VoxelPS_voxeltrace.hlsl"
#include "VoxelPS_sky.hlsl"

float3 LightTrace(float3 origin, float3 startRay, float4 pixelPosition) {
	const uint numRays = scene[0].numRays;
	const float2 screenSize = (float2)scene[0].screenSize;

	VoxelRay primary = VoxelTrace(origin, startRay, -1);
	PrimaryRay pr = { primary.collisionVoxel, primary.side, primary.collision, float3(0.0f, 0.0f, 0.0f) };
	primaryRays[(uint)pixelPosition.y * scene[0].screenSize.x + (uint)pixelPosition.x] = pr;
	
	if (primary.side == -1)
		return SkyTrace(startRay);
	
	float3 totalLight = 0.0f;
	for (uint i = 0; i < numRays; i++) {
		float seed = uint(pixelPosition.y * screenSize.x + pixelPosition.x + (float)i) % 1000;
		float3 ray = 0.0f;
		ray.y = frac(PHI * seed);
		if (primary.normal[primary.side] < 0.0f)
			ray.y = -ray.y;
		float angle = GOLDEN_ANGLE * seed;
		float rad = sqrt(1.0f - (ray.y * ray.y));
		ray.x = cos(angle) * rad;
		ray.z = sin(angle) * rad;
		if (primary.side == 0)
			ray.xyz = ray.yxz;
		else if (primary.side == 2)
			ray.xyz = ray.xzy;
		VoxelRay secondaryRay = VoxelTrace(primary.collision, ray, primary.collisionVoxel);
		if (secondaryRay.side == -1)
			totalLight += SkyTrace(ray) * primary.material.colorEmission.rgb;
		else
			totalLight += secondaryRay.material.lightEmission * primary.material.colorEmission.rgb;
	}
	totalLight /= (float)numRays;
	totalLight += primary.material.lightEmission;
	return totalLight;
}

//float3 LightTrace(float3 origin, float3 startRay, float4 pixelPosition, out VoxelRay primaryRay){
//    //VoxelRay ray;
//    //for (uint i = 0; i < 16; i++) {
//    //    ray = VoxelTrace(origin, startRay, -1);
//    //}
//    
//    
//    const uint maxLayers = 4;
//    const uint maxNumRays = scene[0].maxNumRays;
//    const uint maxHitRays = scene[0].maxHitRays;
//    
//    float angleOffset = (pixelPosition.y * 640.0f + pixelPosition.x) % (2 * PI);
//
//    VoxelRay ray[maxLayers];
//    LightSpread spread[maxLayers - 1];
//    float3 light[maxLayers - 1];
//    light[0] = 0;
//    
//    for (uint i = 0; i < maxLayers - 1; i++){
//        light[i] = 0.0f;
//        ray[i].side = -1;
//    }
//    
//    ray[0].collision = origin;
//    ray[0].normal = startRay;
//    ray[0].collisionVoxel = -1;
//    ray[0].material.colorEmission = float4(1.0f, 1.0f, 1.0f, 1.0f);
//    ray[0].material.lightEmission = 0;
//    ray[0].material.lightSpread = 0;
//    
//    spread[0].Reset(startRay, 0.0f, -1, maxNumRays, maxHitRays);
//    spread[0].Increment(angleOffset);
//    
//    uint c = 1;
//    bool up = true;
//    uint numRays = 0;
//    uint numUps = 0;
//    uint numDowns = 0;
//
//    [loop] while (c > 0) {
//        if (up){
//            numUps++;
//            ray[c] = VoxelTrace(ray[c - 1].collision, spread[c - 1].ray, ray[c - 1].collisionVoxel);
//
//            numRays++;
//            
//            if (ray[c].side != -1) {
//                light[c - 1] += ray[c].material.lightEmission;
//                if (c < maxLayers - 1 && spread[c - 1].childrenRays > 0){
//                    spread[c].Reset(ray[c].normal, ray[c].material.lightSpread, ray[c].side, spread[c - 1].childrenRays, maxHitRays);
//                    light[c] = 0;
//                    c++;
//                }
//            }
//            else {
//                light[c - 1] += SkyTrace(spread[c - 1].ray);
//            }
//        }
//        if (!up) {
//            numDowns++;
//            light[c] /= (float)spread[c].numRays;
//            light[c] *= ray[c].material.colorEmission.rgb;
//            light[c - 1] += light[c];
//        }
//        if (spread[c - 1].Increment(angleOffset)) {
//            up = false;
//            c--;
//        }
//        else {
//            up = true;
//        }
//    }
//    primaryRay = ray[1];
//    //return float3((float)numRays / 255.0f, (float)numUps / 255.0f, (float)numDowns / 255.0f);
//    //return float3((float)numRays / 255.0f, (float)numRays / 255.0f, (float)numRays / 255.0f);
//    return light[0];
//}

#endif