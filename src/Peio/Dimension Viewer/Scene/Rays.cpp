#include "..\App.h"

#include <iostream>

namespace Scene {

	void Rays::Init()
	{
		{
			UINT vecSize = sizeof(float) * App::app.scene.numDims;
			if (!App::app.init || randBuffer.buffer.GetSize() != vecSize * numRandom) {
				std::uniform_real_distribution<float> random(-1.0f, 1.0f);
				std::default_random_engine randomEngine = {};
				vectors.resize(App::app.scene.numDims * numRandom);
				for (UINT i = 0; i < numRandom; i++) {
					MathVector<float> v = RandVector(random, randomEngine);
					memcpy(&vectors[i * App::app.scene.numDims], &v[0], vecSize);
				}
				if (!App::app.init) {
					randBuffer.shaderRegister = 4;
					randBuffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
					App::app.rayRenderer.parameters.push_back(&randBuffer);
				}
				if (!App::app.init || vecSize * numRandom != randBuffer.buffer.GetSize()) {
					randBuffer.Init(vecSize, numRandom, false);
					randBuffer.buffer.SetData((byte*)&vectors[0], vecSize * numRandom);
				}
				std::cout << "Initialized random vectors" << std::endl;
			}
		}
		{
			if (!App::app.init || pixels.size() * sizeof(Pixel) != pixelBuffer.buffer.GetSize()) {
				std::uniform_int_distribution<UINT> random(0, numRandom - 1);
				std::default_random_engine randomEngine = {};
				pixels.resize(App::app.scene.info.screenSize.x() * App::app.scene.info.screenSize.y());
				for (UINT i = 0; i < pixels.size(); i++)
					pixels[i].seed = random(randomEngine);
				if (!App::app.init) {
					pixelBuffer.shaderRegister = 1;
					pixelBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
					App::app.rayRenderer.parameters.push_back(&pixelBuffer);
				}
				if (!App::app.init || pixels.size() * sizeof(Pixel) != pixelBuffer.buffer.GetSize()) {
					pixelBuffer.Init(sizeof(Pixel), (UINT)pixels.size(), false);
					pixelBuffer.buffer.SetData((byte*)&pixels[0], pixels.size() * sizeof(Pixel));
				}
				std::cout << "Initialized seeds" << std::endl;
			}
		}
		{
			if (!App::app.init) {
				perBounceBuffer.shaderRegister = 5;
				perBounceBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
				App::app.rayRenderer.parameters.push_back(&perBounceBuffer);
			}
			if (!App::app.init || perBounce.size() * sizeof(UINT) != perBounceBuffer.buffer.GetSize()) {
				perBounceBuffer.Init(sizeof(UINT), (UINT)perBounce.size(), false);
				perBounceBuffer.buffer.SetData((byte*)&perBounce[0], perBounce.size() * sizeof(UINT));
				std::cout << "Initialized raysPerBounce buffer" << std::endl;
			}
		}
		{
			UINT size = sizeof(float) * App::app.scene.numDims * 2 + sizeof(float) * 3;
			if (!App::app.init) {
				noiseBuffer.shaderRegister = 2;
				noiseBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
				App::app.rayRenderer.parameters.push_back(&noiseBuffer);
				App::app.denoiser.parameters.push_back(&noiseBuffer);
			}
			if (!App::app.init || size * pixels.size() != noiseBuffer.buffer.GetSize()) {
				noiseBuffer.Init(size, (UINT)pixels.size());
				std::cout << "Initialized noise buffer" << std::endl;
			}
		}
		uploaded = false;
	}

	void Rays::Update(ID3D12GraphicsCommandList* cmdList)
	{
		if (!uploaded) {
			randBuffer.Upload(cmdList);
			pixelBuffer.Upload(cmdList);
			perBounceBuffer.Upload(cmdList);
			uploaded = true;
		}
	}

	MathVector<float> Rays::RandVector(std::uniform_real_distribution<float>& random, std::default_random_engine& randomEngine)
	{
		MathVector<float> result(App::app.scene.numDims);
		while (true) {
			float length = 0.0f;
			for (UINT i = 0; i < App::app.scene.numDims; i++) {
				result[i] = random(randomEngine);
				length += result[i] * result[i];
			}
			return result;
		}
	}

}