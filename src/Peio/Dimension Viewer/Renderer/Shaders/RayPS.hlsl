#include "Ray_light.hlsli"

float4 main(VSOutput output) : SV_TARGET
{
	//Normalize(output.sightRay);
	//TracedRay ray;
	//TraceRay(camera[0].position, output.sightRay, ray);
	//if (ray.scale == 1.#INF)
	//	return 0.0f;
	//float m = 5.0f / max(5.0f, ray.scale);
	//if (ray.obj[0] == 0)
	//	return float4(rects[ray.obj[1]].material.emission * m, 1.0f);
	//if (ray.obj[0] == 1)
	//	return float4(spheres[ray.obj[1]].material.emission * m, 1.0f);
	//return 0.0f;
	
	uint2 pixelPosition = (uint2)output.position.xy;
	pixInd = pixelPosition.y * scene[0].screenSize.x + pixelPosition.x;
	DenoisePixel denoisePixel;
	LightTrace(camera[0].position, output.sightRay, denoisePixel);
	denoisePixels[pixInd] = denoisePixel;
	return 0.0f;
	
	//TracedRay ray;
	//TraceRay(camera[0].position, output.sightRay, uint2(-1, -1), ray);
	//if (ray.obj[0] == -1)
	//	return float4(SkyTrace(output.sightRay), 1.0f);
	//
	//float newRay[numDims];
	//float normal[numDims];
	//TracedNormal(ray, normal);
	//Reflect(output.sightRay, normal, newRay);
	//
	//TracedRay ray2;
	//TraceRay(ray.collision, newRay, uint2(-1, -1), ray2);
	//if (ray2.obj[0] == -1)
	//	return float4(1.0f, 0.0f, 0.0f, 1.0f);
	//else
	//	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}