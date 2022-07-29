struct VSOutput {
	float4 pixelPosition : SV_POSITION;
};

struct Scene {
    uint numRays;
    uint2 windowSize;
};

StructuredBuffer<Scene> scene : register(t0);

struct Material {
    float4 color;
    float3 light;
    float spread;
};

StructuredBuffer<Material> materials : register(t1);

struct Ray {
    uint3 voxel;
    uint material;
    uint side;
    float3 light;
};

RWStructuredBuffer<Ray> rays : register(u1);

float4 main(VSOutput input) : SV_TARGET
{
    const float2 offset = abs((float2)scene[0].windowSize / 2.0f - input.pixelPosition);
    if (min(offset.x, offset.y) <= 1.0f && max(offset.x, offset.y) <= 10.0f){
    	return 0.0f;
    }

    int2 size = (int2)scene[0].windowSize;
    int2 p = (int2)input.pixelPosition.xy;
    Ray primary = rays[p.y * size.x + p.x];

    if (primary.material == ~0)
        return float4(primary.light, 1.0f);

    int rad = 5;

    float3 totalLight = 0.0f;
    int numPixels = 0;
    
    for (int y = max(p.y - rad, 0); y <= min(p.y + rad, size.y - 1); y++) {
        for (int x = max(p.x - rad, 0); x <= min(p.x + rad, size.x - 1); x++) {
            Ray ray = rays[y * size.x + x];
            if (ray.material != ~0 && (ray.side == primary.side && ray.voxel[ray.side] == primary.voxel[primary.side])){
                numPixels++;
                totalLight += ray.light;
            }
        }
    }
    totalLight /= (float)numPixels;
    totalLight *= materials[primary.material].color.rgb;
    totalLight += materials[primary.material].light;
    return float4(totalLight, 1.0f);
}