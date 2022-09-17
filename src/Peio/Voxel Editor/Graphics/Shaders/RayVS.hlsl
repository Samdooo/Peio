struct VSInput {
    float2 position : POSITION;
};

struct VSOutput {
    float4 pixelPosition : SV_POSITION;
    float3 cameraPosition : CAMERA_POSITION;
    float3 sightRay : SIGHT_RAY;
};

float3 RotateX(float3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return float3(p.x, c * p.y + s * p.z, -s * p.y + c * p.z);
}

float3 RotateY(float3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return float3(c * p.x - s * p.z, p.y, s * p.x + c * p.z);
}

struct Camera {
    float3 position;
    float2 rotation;
    float fov;
    float aspectRatio;
};
StructuredBuffer<Camera> camera : register(t0);
//cbuffer CameraBuffer : register(t0) {
//    Camera camera;
//}

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pixelPosition = float4(input.position, 1.0f, 1.0f);
    output.cameraPosition = camera[0].position;
    output.sightRay = float3(input.position.x, input.position.y * camera[0].aspectRatio, 1.0f / tan(camera[0].fov / 2.0f));
    output.sightRay = RotateX(output.sightRay, camera[0].rotation.y);
    output.sightRay = RotateY(output.sightRay, camera[0].rotation.x);
    return output;
}