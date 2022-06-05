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
	bool useColor : USE_COLOR, useTexture : USE_TEXTURE, useAlpha : USE_ALPHA;
};

struct Window {
	uint2 size;
};
cbuffer WindowBuffer : register(b0) {
	Window window;
}

struct Rectangle {
	uint2 offset;
	uint2 position;
	uint2 size;
	bool useColor, useTexture, useAlpha;
};
cbuffer RectangleBuffer : register(b1) {
	Rectangle rectangle;
}

VSOutput main(VSInput input)
{
	VSOutput output;
	float2 position = (float2)(rectangle.offset + rectangle.position) + ((float2)rectangle.size * input.position);
	position /= (float2)window.size;
	position *= 2.0f;
	position.x = position.x - 1.0f;
	position.y = 1.0f - position.y;
	output.position = float4(position, 0.0f, 1.0f);
	output.color = input.color;
	output.texCoord = input.texCoord;
	output.alphaCoord = input.alphaCoord;
	output.useColor = rectangle.useColor;
	output.useTexture = rectangle.useTexture;
	output.useAlpha = rectangle.useAlpha;
	return output;
}