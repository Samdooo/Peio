struct VSInput {
	float2 position : POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
	float2 alphaCoord : ALPHACOORD;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float4 color : PS_COLOR;
	float2 texCoord : PS_TEXCOORD;
	float2 alphaCoord : PS_ALPHACOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 0.0f, 1.0f);
	output.color = input.color;
	output.texCoord = input.texCoord;
	output.alphaCoord = input.alphaCoord;
	return output;
}