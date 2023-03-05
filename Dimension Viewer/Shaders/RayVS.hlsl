#include "Ray_scene.hlsli"
#include "Ray_math.hlsli"

VSOutput main(float2 pos : POSITION) {
	VSOutput output;
    output.position = float4(pos, 0.0f, 1.0f);
    output.sightRay[0] = 1.0f / tan(camera[0].fov * 0.5f);
    output.sightRay[1] = pos.y * camera[0].aspectRatio;
    output.sightRay[2] = pos.x;
    [unroll(numDims - 3)] for (uint i = 3; i < numDims; i++)
        output.sightRay[i] = 0.0f;
    Rotate(output.sightRay, camera[0].rotation);
    //Rotate2D2(output.sightRay, 0, camera[0].rotation[0]);
    return output;
}