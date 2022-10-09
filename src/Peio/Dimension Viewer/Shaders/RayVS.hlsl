#include "Ray_input.hlsli"

VSOutput main(float2 position : POSITION)
{
	VSOutput output;
	output.position = float4(position, 0.0f, 1.0f);

	output.ray.v[0] = 1.0f / tan(camera[0].fov / 2.0f);
	output.ray.v[1] = position.y * camera[0].aspectRatio;
	output.ray.v[2] = position.x;
	[unroll] for (uint i = 3; i < numDims; i++)
		output.ray.v[i] = 0.0f;
	
	[unroll] for (uint i = 0; i < numDims - 1; i++) {
		float x = output.ray.v[0] * cos(camera[0].rotation.v[i]) - output.ray.v[i + 1] * sin(camera[0].rotation.v[i]);
		float y = output.ray.v[i + 1] * cos(camera[0].rotation.v[i]) + output.ray.v[0] * sin(camera[0].rotation.v[i]);
		output.ray.v[0] = x;
		output.ray.v[i + 1] = y;
	}

	return output;
}