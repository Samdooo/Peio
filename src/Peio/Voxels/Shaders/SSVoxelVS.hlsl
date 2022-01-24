struct VSInput {
    float2 position : POSITION;
    float3 cameraPosition : CAMERA_POSITION;
    float2 rotation : ROTATION;
    float fov : FOV;
    float aspectRatio : ASPECT_RATIO;
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

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pixelPosition = float4(input.position, 1.0f, 1.0f);
    output.cameraPosition = input.cameraPosition;
    output.sightRay = float3(input.position.x, input.position.y * input.aspectRatio, 1.0f / tan(input.fov / 2.0f));
    output.sightRay = RotateX(output.sightRay, input.rotation.y);
    output.sightRay = RotateY(output.sightRay, input.rotation.x);
    return output;
}