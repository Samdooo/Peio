#include "..\App.h"

namespace Scene {

	void Rays::Init()
	{
		{
			UINT vecSize = sizeof(float) * App::app.scene.numDims;
			std::uniform_real_distribution<float> random(-1.0f, 1.0f);
			std::default_random_engine randomEngine = {};
			vectors.resize(App::app.scene.numDims * numRandom);
			for (UINT i = 0; i < numRandom; i++) {
				MathVector<float> v = RandVector(random, randomEngine);
				memcpy(&vectors[i * App::app.scene.numDims], &v[0], vecSize);
			}
			randBuffer.Init(vecSize, numRandom, false);
			randBuffer.buffer.SetData((byte*)&vectors[0], vecSize * numRandom);
			randBuffer.shaderRegister = 4;
			randBuffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
			App::app.rayRenderer.parameters.push_back(&randBuffer);
		}
		{
			std::uniform_int_distribution<UINT> random(0, numRandom - 1);
			std::default_random_engine randomEngine = {};
			pixels.resize(App::app.scene.info.screenSize.x() * App::app.scene.info.screenSize.y());
			for (UINT i = 0; i < pixels.size(); i++)
				pixels[i].seed = random(randomEngine);
			pixelBuffer.Init(sizeof(Pixel), (UINT)pixels.size(), false);
			pixelBuffer.buffer.SetData((byte*)&pixels[0], pixels.size() * sizeof(Pixel));
			pixelBuffer.shaderRegister = 1;
			pixelBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
			App::app.rayRenderer.parameters.push_back(&pixelBuffer);
		}
		{
			perBounceBuffer.Init(sizeof(UINT), (UINT)perBounce.size(), false);
			perBounceBuffer.buffer.SetData((byte*)&perBounce[0], perBounce.size() * sizeof(UINT));
			perBounceBuffer.shaderRegister = 5;
			perBounceBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
			App::app.rayRenderer.parameters.push_back(&perBounceBuffer);
		}
		{
			UINT size = sizeof(float) * App::app.scene.numDims * 2 + sizeof(float) * 3;
			noiseBuffer.Init(size, (UINT)pixels.size());
			noiseBuffer.shaderRegister = 2;
			noiseBuffer.visibility = D3D12_SHADER_VISIBILITY_PIXEL;
			App::app.rayRenderer.parameters.push_back(&noiseBuffer);
			App::app.denoiser.parameters.push_back(&noiseBuffer);
		}
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