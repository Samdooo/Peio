struct VSOutput {
	float4 position : SV_POSITION;
	float4 color : PS_COLOR;
	float2 texCoord : PS_TEXCOORD;
	float2 alphaCoord : PS_ALPHACOORD;
};

struct Rectangle {
	bool useColor, useTexture, useAlpha;
};
cbuffer RectangleBuffer : register(b0) {
	Rectangle rectangle;
}

Texture2D<float4> tex : register(t0);
SamplerState texSampler : register(s0);

Texture2D<float> alpha : register(t1);
SamplerState alphaSampler : register(s1);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (rectangle.useColor) {
		output *= input.color;
	}
	if (rectangle.useTexture) {
		output *= tex.Sample(texSampler, input.texCoord);
	}
	if (rectangle.useAlpha) {
		output.a *= alpha.Sample(alphaSampler, input.alphaCoord);
	}
	return output;
}