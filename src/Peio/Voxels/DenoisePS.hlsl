Texture2D tex : register(t0);
SamplerState state : register(s0);

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float2 size : SIZE;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 col = tex.Sample(state, input.texCoord);
	return col;
}