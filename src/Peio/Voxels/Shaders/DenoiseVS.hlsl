struct VSOutput {
    float4 pixelPosition : SV_POSITION;
};

VSOutput main(float2 pos : POSITION)
{
	VSOutput output;
	output.pixelPosition = float4(pos, 0.0f, 1.0f);
	return output;
}