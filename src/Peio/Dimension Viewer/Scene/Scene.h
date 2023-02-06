#pragma once

#include "Objects.h"
#include "Camera.h"
#include "Rays.h"

namespace Scene {

	struct Scene {

		UINT numDims = 3;
		UINT maxBounces = 8;

		struct Info {
			Uint2 screenSize = { 1280, 720 };
			UINT numRects = 0;
			UINT numSpheres = 0;
			UINT numRandom = 0;
			struct Denoiser {
				UINT radius = 0;
				float maxDist = 0.0f;
				float maxAngle = 0.0f;
			} denoiser = {};
		} info = {};
		Gfx::ArrayBufferParameter infoBuffer = {};

		Objects objects = {};
		Camera camera = {};
		Rays rays = {};

		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

	};

}