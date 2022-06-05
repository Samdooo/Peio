struct VSOutput {
	float4 position : SV_POSITION;
	float4 color : PS_COLOR;
	float2 texCoord : PS_TEXCOORD;
	float2 alphaCoord : PS_ALPHACOORD;
	bool useColor : USE_COLOR, useTexture : USE_TEXTURE, useAlpha : USE_ALPHA;
};

Texture2D tex : register(t0);
SamplerState texSampler : register(s0);

Texture2D alpha : register(t1);
SamplerState alphaSampler : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (input.useColor) {
		output *= input.color;
	}
	if (input.useTexture) {
		output *= tex.Sample(texSampler, input.texCoord);
	}
	if (input.useAlpha) {
		output.a *= alpha.Sample(alphaSampler, input.alphaCoord).a;
	}
	return output;
}