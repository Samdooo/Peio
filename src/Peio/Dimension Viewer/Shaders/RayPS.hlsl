#include "RayPS_light.hlsli"

float4 main(VSOutput input) : SV_TARGET {
	//float3 light = Light(camera[0].position, input.ray, (uint2)input.position.xy);
	//return float4(light, 1.0f);
	Light(camera[0].position, input.ray, (uint2)input.position.xy);
	return 0.0f;
}