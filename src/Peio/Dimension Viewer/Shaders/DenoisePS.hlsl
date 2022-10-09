#include "Ray_input.hlsli"
#include "Ray_math.hlsli"

float4 main(float4 position : SV_POSITION) : SV_TARGET {
	int2 pixelPosition = (int2)position.xy;
	NoiseRay noiseRay = noiseRays[pixelPosition.y * scene[0].windowSize.x + pixelPosition.x];
	
	if (noiseRay.scale == 1.#INF)
		return float4(noiseRay.emission, 1.0f);
	
	uint numPixels = 0;
	uint rad = 10;
	float avgDiffuse = 0.0f;
	
	[loop] for (int y = max(pixelPosition.y - rad, 0); y <= min(pixelPosition.y + rad, scene[0].windowSize.y - 1); y++){
		[loop] for (int x = max(pixelPosition.x - rad, 0); x <= min(pixelPosition.x + rad, scene[0].windowSize.x - 1); x++){
			NoiseRay cur = noiseRays[y * scene[0].windowSize.x + x];
			if (cur.scale != 1.#INF){
				if (Cosine(noiseRay.normal, cur.normal) >= 0.9f){
					avgDiffuse += cur.diffuseLight;
					numPixels++;
				}
			}
		}
	}
	avgDiffuse /= (float)numPixels;
	return float4(noiseRay.emission + (1.0f - noiseRay.spectral) * avgDiffuse * noiseRay.reflection, 1.0f);
}