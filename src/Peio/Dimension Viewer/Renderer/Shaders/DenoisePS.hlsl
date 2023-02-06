#include "Ray_scene.hlsli"
#include "Ray_math.hlsli"

float4 main(float4 position : SV_POSITION) : SV_TARGET
{
	int2 pixelPosition = (uint2)position.xy;
	int pixInd = pixelPosition.y * scene[0].screenSize.x + pixelPosition.x;
	
	DenoisePixel pixel = denoisePixels[pixInd];
	if (pixel.position[0] == 1.#INF)
		return float4(pixel.light, 1.0f);
	
	const int rad = (int)scene[0].denoiser.radius;
	const float maxDist = scene[0].denoiser.maxDist;
	const float maxSin = sin(scene[0].denoiser.maxAngle);

	uint numLights = 0;
	float3 light = 0.0f;
	for (int y = max(0, pixelPosition.y - rad); y <= min(scene[0].screenSize.y - 1, pixelPosition.y + rad); y++){
		for (int x = max(0, pixelPosition.x - rad); x <= min(scene[0].screenSize.x - 1, pixelPosition.x + rad); x++){
			DenoisePixel that = denoisePixels[y * scene[0].screenSize.x + x];
			if (that.position[0] == 1.#INF)
				continue;
			float diff[numDims];
			[unroll(numDims)] for (uint i = 0; i < numDims; i++)
				diff[i] = that.position[i] - pixel.position[i];
			float dist = GetLength(diff);
			if (dist > maxDist)
				continue;
			if (VectorSin(pixel.normal, that.normal) > maxSin)
				continue;
			light += that.light;
			numLights++;
		}
	}
	light /= (float)numLights;
	return float4(light, 1.0f);
}