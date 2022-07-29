#include "VoxelPS_lighttrace.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
	//const float2 offset = abs((float2)scene[0].windowSize / 2.0f - input.pixelPosition);
	//if (min(offset.x, offset.y) <= 1.0f && max(offset.x, offset.y) <= 10.0f){
	//	return 0.0f;
	//}

	LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition.xy);
	//discard;
	return 0.0f;

	//VoxelRay ray = VoxelTrace(input.cameraPosition, input.sightRay, uint3(-1, -1, -1));
	//if (ray.material == ~0)
	//	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//else {
	//	//float3 off = float3(0.5f, 0.5f, 0.5f) + (ray.collision - ((float3)ray.voxel + float3(0.5f, 0.5f, 0.5f)));
	//	//return float4(off, 1.0f);
	//	//
	//	VoxelRay second = VoxelTrace(ray.collision, ray.normal, uint3(-1, -1, -1));
	//	if (second.material == ~0)
	//		return float4(0.0f, 0.0f, 1.0f, 1.0f);
	//	else
	//		return materials[ray.material].color;
	//}
	//return float4(LightTrace(input.cameraPosition, input.sightRay, input.pixelPosition), 1.0f);
	//VoxelRay ray = VoxelTrace(input.cameraPosition, input.sightRay, uint3(~0, ~0, ~0));
	//if (ray.material == ~0)
	//	return float4(SkyTrace(input.sightRay), 1.0f);
	//else
	//	return materials[ray.material].color;
}

//struct VSOutput {
//    float4 pixelPosition : SV_POSITION;
//    float3 cameraPosition : CAMERA_POSITION;
//    float3 sightRay : SIGHT_RAY;
//};
//
//struct MaterialGroup {
//    uint indices[2][2][2];
//};
//
//StructuredBuffer<MaterialGroup> tree : register(t1);
//
//struct Material {
//    float4 color;
//    float3 light;
//    float spread;
//};
//
//StructuredBuffer<Material> materials : register(t2);
//
//float3 SkyTrace(float3 ray) {
//    const float3 sunRay = float3(1.0f, 1.0f, 1.0f);
//    //const float3 sunRay = float3(0.0f, 0.0f, 1.0f);
//    ray = normalize(ray);
//
//    float3 diff = length(ray - sunRay);
//    return float3(0.5f, 0.8f, 0.9f) + float3(10.0f, 10.0f, 9.0f) * max((1.0f - diff), 0.0f);
//}
//
//float4 main(VSOutput input) : SV_TARGET
//{
//   // return float4(input.sightRay.y, 0, 0, 1);
//
//    const double3 origin = input.cameraPosition;
//    const double3 ray = input.sightRay;
//    const double3 invRay = 1.0f / ray;
//
//    const uint numLayers = 8;
//
//    double curScale = 0.0f;
//
//    { // Trace to the root cube
//        double rad = (float)(1 << numLayers) / 2.0f;
//        double3  mid = double3 (rad, rad, rad) - origin;
//        double3  minDiv = mid * invRay - abs(invRay * rad);
//        double3  maxDiv = mid * invRay + abs(invRay * rad);
//
//        double minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0f);
//        double maxScale = min(min(maxDiv.x, maxDiv.y), maxDiv.z);
//
//        if (minScale > maxScale) {
//            return float4(SkyTrace(input.sightRay), 1.0f);
//        }
//        curScale = minScale;
//    }
//
//    uint3 path = 0;
//    uint curLayer = 0;
//    uint indices[numLayers];
//    uint nextIndex = 0;
//    bool down = true;
//
//    uint maxLayer = 0;
//    [loop] while (true) {
//        if (curLayer > maxLayer)
//            maxLayer = curLayer;
//        uint mask = 1U << (numLayers - curLayer - 1);
//        double rad = (float)(1U << (numLayers - curLayer)) / 2.0f;
//        double3 mid = (double3)(path & uint3(~mask, ~mask, ~mask)) + double3(rad, rad, rad);
//
//        if (down) {
//            if (curLayer == numLayers) {
//                return materials[nextIndex].color;
//            }
//
//            double3 curPos = origin + ray * curScale;
//            indices[curLayer] = nextIndex;
//            [unroll(3)] for (uint i = 0; i < 3; i++) {
//                if (curPos[i] >= mid[i])
//                    path[i] |= mask;
//                else
//                    path[i] &= ~mask;
//            }
//        }
//        else {
//
//            uint axis = -1;
//            double bestScale = 1.#INF;
//            [unroll(3)] for (uint i = 0; i < 3; i++) {
//                double s = (mid[i] - origin[i]) * invRay[i];
//                if (s <= curScale) // Might cause errors because of double inaccuracy (?)
//                    continue;
//                if (s < bestScale) {
//                    axis = i;
//                    bestScale = s;
//                }
//            }
//
//            bool inside = true;
//            [unroll(3)] for (uint i = 0; i < 3; i++) {
//                if (abs((origin[i] + ray[i] * bestScale) - mid[i]) > rad) {
//                    inside = false;
//                    break;
//                }
//            }
//
//            if (axis == -1 || !inside) {
//                if (curLayer == 0)
//                    break;
//                [unroll(3)] for (uint i = 0; i < 3; i++)
//                    path &= ~mask;
//                curLayer--;
//                continue;
//            }
//            curScale = bestScale;
//            if (axis == 0)
//                path.x ^= mask;
//            else if (axis == 1)
//                path.y ^= mask;
//            else
//                path.z ^= mask;
//        }
//        nextIndex = tree[indices[curLayer]].indices[(bool)(path.x & mask)]
//            [(bool)(path.y & mask)][(bool)(path.z & mask)];
//        if (nextIndex == -1) {
//            down = false;
//            continue;
//        }
//        else {
//            down = true;
//            curLayer++;
//        }
//    }
//    return float4(SkyTrace(input.sightRay), 1.0f);
//}