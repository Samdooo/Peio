#pragma once

#include "..\..\Graphics\SubresourceBuffer.h"
#include "..\..\Graphics\UnorderedAccessView.h"

namespace Scene {

	struct RayField {

		struct Ray {
			//Peio::Uint3 voxel = {};
			UINT voxel = 0;
			UINT material = 0;
			UINT side = 0;
			Peio::Float3 light = {};
			UINT numRays = 0;
			UINT seed = 0;
		};

		void Init();
		void Upload();

		const UINT maxSeed = (1 << 16);

		Peio::Gfx::SubresourceBuffer<Ray> buffer = {};
		Peio::Gfx::BufferUAV uav = {};

	};

}