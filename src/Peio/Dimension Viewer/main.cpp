#include "..\Graphics\MediaGraphics.h"
#include "..\Media\Encoder.h"
#include "..\Media\Remux.h"
#include "..\Voxels\FullscreenRenderer.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\UnorderedAccessView.h"

#include <iostream>
#include <random>

template <UINT numDims>
struct Camera {
	Peio::Array<float, numDims> position;
	Peio::Array<float, numDims - 1> rotation;
	float fov;
	float aspectRatio;
};

struct Scene {
	UINT numRands;
	Peio::Uint2 windowSize;
	UINT numRects;
	UINT numSpheres;
	Peio::Float3 backLight;
	UINT numBounces;
};

struct Material {
	Peio::Float3 emission;
	Peio::Float3 reflection;
	float spectral;
};

template <UINT numDims>
struct HyperRect {
	Peio::Array<float, numDims> position;
	Peio::Array<float, numDims> size;
	Material material;
};

template <UINT numDims>
struct HyperSphere {
	Peio::Array<float, numDims> center;
	float radius;
	Material material;
};

template <UINT numDims>
struct NoiseRay {
	float scale;
	Peio::Array<float, numDims> normal;
	Peio::Float3 diffuseLight;
	Peio::Float3 reflection;
	Peio::Float3 emission;
	float spectral;
};

std::uniform_real_distribution<float> random(-1.0f, 1.0f);
std::default_random_engine randomGen = {};
template <UINT numDims>
Peio::Array<float, numDims> RandVector() {
	Peio::Array<float, numDims> result;
	while (true) {
		float dist = 0.0f;
		for (UINT i = 0; i < numDims; i++) {
			result[i] = random(randomGen);
			dist += result[i] * result[i];
		}
		if (dist <= 1.0f) {
			if (dist == 0.0f)
				result[0] = 1.0f;
			break;
		}
	}
	return result;
}

int main() {

	const UINT numRands = 4096;
	const Peio::Long2 size = { 1920, 1080 };

	Peio::Gfx::Init();

	Peio::Med::Encoder encoder;
	encoder.Init(AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, size, 60, 100'000'000);
	
	std::ofstream ofile("out.h264", std::ios::binary);
	std::function<void(const Peio::byte*, int)> onPacket = [&ofile](const Peio::byte* data, int length) {
		ofile.write((const char*)data, length);
	};

	Peio::Gfx::MediaGraphics graphics;
	graphics.Init(size, 1);
	
	Peio::Vxl::FullscreenRenderer renderer;
	renderer.Init(graphics.GetCommandList(), "Resources/Shaders/RayVS.cso", "Resources/Shaders/RayPS.cso",
		{ 
		D3D12_SHADER_VISIBILITY_PIXEL, // Scene
		D3D12_SHADER_VISIBILITY_ALL,   // Camera
		D3D12_SHADER_VISIBILITY_PIXEL, // Ray per pixel
		D3D12_SHADER_VISIBILITY_PIXEL, // Hyperrects
		D3D12_SHADER_VISIBILITY_PIXEL, // Hyperspheres
		D3D12_SHADER_VISIBILITY_PIXEL, // Seeds
		D3D12_SHADER_VISIBILITY_PIXEL  // Random vectors
		}, 
		{
		D3D12_SHADER_VISIBILITY_PIXEL // Noise rays
		});

	UINT numFrames = 60 * 5;
	const UINT numDims = 3;

	Peio::Gfx::SubresourceBuffer<Scene> sceneBuffer;
	sceneBuffer.Allocate(1);
	sceneBuffer[0] = {
		numRands, size, 1, 2, { 0.3f, 0.3f, 0.33f }, 3
	};
	Peio::Gfx::BufferSRV sceneSrv;
	sceneSrv.Init(sceneBuffer.GetBufferSize(), sceneBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	sceneSrv.Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<Camera<numDims>> cameraBuffer;
	cameraBuffer.Allocate(1);
	cameraBuffer[0] = {
		{ -1.0f, 3.0f, 4.0f }, { 0.0f, -0.4f }, 3.14f / 2.0f, (float)size.y() / (float)size.x()
	};
	Peio::Gfx::BufferSRV cameraSrv;
	cameraSrv.Init(cameraBuffer.GetBufferSize(), cameraBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	cameraSrv.Upload(cameraBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<UINT> raysPerBounceBuffer;
	raysPerBounceBuffer.Allocate(3);
	raysPerBounceBuffer[0] = 1024;
	raysPerBounceBuffer[1] = 4;
	raysPerBounceBuffer[2] = 2;
	Peio::Gfx::BufferSRV raysPerBounceSrv;
	raysPerBounceSrv.Init(raysPerBounceBuffer.GetBufferSize(), raysPerBounceBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	raysPerBounceSrv.Upload(raysPerBounceBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<HyperRect<numDims>> rectBuffer;
	rectBuffer.Allocate(1);
	rectBuffer[0] = {
		{ 5.0f, 0.0f, 0.0f },
		{ 100.0f, 1.0f, 50.0f },
		{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.8f, 0.8f },
			0.0f
		}
	};
	//rectBuffer[1] = {
	//	{ -5.0f, -5.0f, -5.0f },
	//	{ 15.0f, 10.0f, 15.0f },
	//	{
	//		{ 0.2f, 0.2f, 0.2f },
	//		{ 0.6f, 0.6f, 0.6f },
	//		0.0f
	//	}
	//};
	Peio::Gfx::BufferSRV rectSrv;
	rectSrv.Init(rectBuffer.GetBufferSize(), rectBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	rectSrv.Upload(rectBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<HyperSphere<numDims>> sphereBuffer;
	sphereBuffer.Allocate(3);
	sphereBuffer[0] = {
		{ 0.0f, 2.5f, 4.0f },
		0.4f,
		{
			{ 10.0f, 10.0f, 10.0f },
			{ 0.0f, 0.0f, 0.0f },
			0.0f
		}
	};
	sphereBuffer[1] = {
		{ 6.0f, 2.0f, 2.0f },
		0.4f,
		{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.7f, 0.7f, 0.0f },
			0.0f
		}
	};
	sphereBuffer[2] = {
		{ 0.0f, 0.0f, 0.0f },
		8.0f,
		{
			{ 0.1f, 0.1f, 0.1f },
			{ 0.6f, 0.6f, 0.6f },
			0.5f
		}
	};
	Peio::Gfx::BufferSRV sphereSrv;
	sphereSrv.Init(sphereBuffer.GetBufferSize(), sphereBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	sphereSrv.Upload(sphereBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<UINT> seedBuffer;
	seedBuffer.Allocate(size.x() * size.y());
	std::uniform_int_distribution<UINT> random(0, numRands - 1);
	std::default_random_engine randomGen = {};
	for (UINT i = 0; i < size.y(); i++) {
		for (UINT j = 0; j < size.x(); j++) {
			seedBuffer[i * size.x() + j] = random(randomGen);
		}
	}
	Peio::Gfx::BufferSRV seedSrv;
	seedSrv.Init(seedBuffer.GetBufferSize(), seedBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	seedSrv.Upload(seedBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::SubresourceBuffer<Peio::Array<float, numDims>> vectorBuffer;
	vectorBuffer.Allocate(numRands);
	for (UINT i = 0; i < vectorBuffer.GetNumElements(); i++)
		vectorBuffer[i] = RandVector<numDims>();
	Peio::Gfx::BufferSRV vectorSrv;
	vectorSrv.Init(vectorBuffer.GetBufferSize(), vectorBuffer.GetNumElements(), D3D12_RESOURCE_STATE_GENERIC_READ);
	vectorSrv.Upload(vectorBuffer.GetResourceData(), graphics.GetCommandList());

	Peio::Gfx::BufferUAV noiseRayUav;
	noiseRayUav.Init(size.x() * size.y() * sizeof(NoiseRay<numDims>), size.x() * size.y(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	Peio::Vxl::FullscreenRenderer denoiser;
	denoiser.Init(graphics.GetCommandList(), "Resources/Shaders/DenoiseVS.cso", "Resources/Shaders/DenoisePS.cso",
		{
		D3D12_SHADER_VISIBILITY_PIXEL // Scene
		},
		{
		D3D12_SHADER_VISIBILITY_PIXEL // Noise rays
		}
	);

	for (UINT i = 0; i < numFrames; i++) {
		graphics.Clear({});
		
		renderer.Prepare(graphics.GetCommandList(), { 0.0f, 0.0f, (float)size.x(), (float)size.y(), 0.0f, 1.0f}, {0, 0, size.x(), size.y()});
		renderer.SetSRV(graphics.GetCommandList(), 0, sceneSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 1, cameraSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 2, raysPerBounceSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 3, rectSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 4, sphereSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 5, seedSrv.GetGPUAddress());
		renderer.SetSRV(graphics.GetCommandList(), 6, vectorSrv.GetGPUAddress());
		renderer.SetUAV(graphics.GetCommandList(), 0, noiseRayUav.GetGPUAddress());
		renderer.Render(graphics.GetCommandList());

		denoiser.Prepare(graphics.GetCommandList(), { 0.0f, 0.0f, (float)size.x(), (float)size.y(), 0.0f, 1.0f }, { 0, 0, size.x(), size.y() });
		denoiser.SetSRV(graphics.GetCommandList(), 0, sceneSrv.GetGPUAddress());
		denoiser.SetUAV(graphics.GetCommandList(), 0, noiseRayUav.GetGPUAddress());
		denoiser.Render(graphics.GetCommandList());

		graphics.Render();

		Peio::Med::Frame frame;
		frame.Init(graphics.GetBuffer(), size, AV_PIX_FMT_RGBA);
		encoder.EncodeFrame(&frame, onPacket);

		//cameraBuffer[0].position.x() += 0.03f;
		//cameraBuffer[0].position.z() += 0.05f;
		//cameraBuffer[0].position.y() += 0.1f;
		//cameraBuffer[0].rotation.x() -= 0.008f;
		//cameraBuffer[0].rotation.y() -= 0.003f;
		cameraSrv.Upload(cameraBuffer.GetResourceData(), graphics.GetCommandList());

		//sphereBuffer[0].center.y() += 0.05f;
		sphereBuffer[0].center.x() += 0.05f;
		sphereSrv.Upload(sphereBuffer.GetResourceData(), graphics.GetCommandList());

		std::cout << "Rendered and encoded frame #" << (i + 1) << "/" << numFrames << std::endl;
	}
	encoder.Flush(onPacket);
	ofile.close();
	
	Peio::Med::Remux("out.h264", "out.mp4");

	return 0;
}