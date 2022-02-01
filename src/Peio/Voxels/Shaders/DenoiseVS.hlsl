struct VS_INPUT {
	float2 position : POSITION;
	float2 texCoord : TEXCOORD;
	float2 size : SIZE;
};
struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float2 size : SIZE;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position.x, input.position.y, 0.0f, 1.0f);
	output.texCoord = input.texCoord;
	output.size = input.size;
	return output;
}