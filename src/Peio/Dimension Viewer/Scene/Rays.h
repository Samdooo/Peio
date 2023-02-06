#pragma once

#include <random>
#include "..\MathVector.h"
#include "..\..\Graphics\InputParameter.h"

namespace Scene {

	struct Rays {

		struct Pixel {
			UINT seed = 0;
		};

		UINT numRandom = 4096;
		std::vector<float> vectors = {};
		std::vector<Pixel> pixels = {};
		std::vector<UINT> perBounce = {};

		Gfx::ArrayBufferParameter randBuffer = {};
		Gfx::ArrayBufferParameter perBounceBuffer = {};
		Gfx::RWArrayBufferParameter pixelBuffer = {};
		Gfx::RWArrayBufferParameter noiseBuffer = {};
		bool uploaded = false;

		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

		MathVector<float> RandVector(std::uniform_real_distribution<float>& random, std::default_random_engine& randomEngine);

	};

}