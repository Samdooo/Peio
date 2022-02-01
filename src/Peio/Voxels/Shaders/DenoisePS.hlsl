Texture2D tex : register(t0);
SamplerState state : register(s0);

#define GOLDEN_ANGLE 2.399963229728653f

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float2 size : SIZE;
};

bool Sign(float x) {
	return !bool(asuint(x) & (1 << 31));
}

const uint NegZero() {
	return asfloat(asuint(0.0f) | (1 << 31));
}

int GetSide(in out float4 col) {
	int side = int(col.r < 0.0f || col.r == 1.#INF) + 2 * int(col.g < 0.0f || col.g == 1.#INF) - 1;
	col.rg = abs(col.rg);
	if (col.r == 1.#INF)
		col.r = 0.0f;
	if (col.g == 1.#INF)
		col.g = 0.0f;
	return side;
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	const int radius = 10;
	const float2 invSize = 1.0f / input.size;

	float4 primary = tex.Sample(state, input.texCoord * invSize);
	int side = GetSide(primary);

	float4 total = 0.0f;
	float count = 0.0f;

	//return float4(primary.a / 10.0f, primary.a / 10.0f, primary.a / 10.0f, 1.0f);

	[loop] for (int y = max((int)input.texCoord.y - radius, 0); y <= min((int)input.texCoord.y + radius, (int)input.size.y - 1); y++) {
		[loop] for (int x = max((int)input.texCoord.x - radius, 0); x <= min((int)input.texCoord.x + radius, (int)input.size.x - 1); x++) {
			float4 col = tex.Sample(state, float2((float)x, (float)y) * invSize);
			
			int tmpSide = int(col.r < 0.0f || col.r == 1.#INF) + 2 * int(col.g < 0.0f || col.g == 1.#INF) - 1;
			
			if (col.r < 0.0f)
				col.r = -col.r;
			else if (col.r == 1.#INF)
				col.r = 0.0f;
			if (col.g < 0.0f)
				col.g = -col.g;
			else if (col.g == 1.#INF)
				col.g = 0.0f;

			if (tmpSide == side && abs(col.a - primary.a) <= 0.001f) {
				total += col;
				count += 1.0f;
			}

			//col.rg = abs(col.rg);
			//if (col.r == 1.#INF)
			//	col.r = 0.0f;
			//if (col.g == 1.#INF)
			//	col.g = 0.0f;
			//if (/*tmpSide == side && */col.a == primary.a) {
			//	total += col;
			//	count += 1.0f;
			//}
		}
	}
	total /= count;
	total.a = 1.0f;
	return total;
	//return float4(primary.a / 10.0f, 0.0f, 0.0f, 1.0f);

	//return float4(abs(primary.rgb), 1.0f);

	//}
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);

	//uint4 test = tex.Sample(state, input.texCoord * fSize);
	//return float4(test, (float)asuint(test) / 255.0f, 0.0f, 1.0f);

	//if (test == asfloat(5))
	//	return float4(0.0f, 1.0f, 0.0f, 1.0f);
	//else
	//	return float4(1.0f, 0.0f, 0.0f, 1.0f);
	
	//return float4((float)voxel / 255.0f, (float)voxel / 2550.0f, (float)voxel / 25500.0f, 1.0f);

}