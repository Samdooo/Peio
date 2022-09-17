#include "RayPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
	LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition.xy);
	return 0.0f;
}