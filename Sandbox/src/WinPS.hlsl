StructuredBuffer<float4> color : register(t0);

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	return color[0];
}