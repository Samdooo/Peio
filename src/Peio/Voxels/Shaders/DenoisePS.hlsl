struct VS_OUTPUT {
	float4 pixelPosition : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float2 size : SIZE;
};

struct PrimaryRay {
	uint collisionVoxel;
	int side; // -1 indicates no collision
	float3 collision;
	float3 light;
};

RWStructuredBuffer<PrimaryRay> primaryRays : register(u1);

float4 main(VS_OUTPUT input) : SV_TARGET
{
	const int2 pos = int2((int)input.pixelPosition.x, (int)input.pixelPosition.y);
	const int2 size = int2((int)input.size.x, (int)input.size.y);

	//float3 light = primaryRays[pos.y * size.x + pos.x].light;
	float3 light = 0.0f;

	int rad = 16;
	for (int i = max(pos.y - rad, 0); i <= min(pos.y + rad, size.y - 1); i++) {
		for (int j = max(pos.x - rad, 0); j <= min(pos.x + rad, size.x - 1); j++) {
			light.r += max(primaryRays[i * size.x + j].light.r - 1.0f, 0.0f);
			light.g += max(primaryRays[i * size.x + j].light.g - 1.0f, 0.0f);
			light.b += max(primaryRays[i * size.x + j].light.b - 1.0f, 0.0f);
		}
	}
	light /= (rad + rad + 1) * (rad + rad + 1);
	light += primaryRays[pos.y * size.x + pos.x].light;
	return float4(light, 1.0f);

	//return float4(primaryRays[(uint)input.pixelPosition.y * (uint)input.size.x + (uint)input.pixelPosition.x].light, 1.0f);
}