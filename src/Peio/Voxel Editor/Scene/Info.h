#pragma once

#include "..\..\Graphics\SubresourceBuffer.h"
#include "..\..\Graphics\ShaderResourceView.h"
#include "..\..\Voxels\Camera.h"

namespace Scene {

	struct Info {

		struct Scene {
			struct Sky {
				Peio::Float2 sunRotation = {};
			};
			UINT numRays = 0;
			UINT denoiseRadius = 0;
			Peio::Uint2 windowSize = {};
			Sky sky = {};
			BOOL newRays = TRUE;
			_NODISCARD bool operator==(const Scene& that) const;
		};

		void Init();
		void Upload();
		void Update();

		Scene scene = {};
		Peio::Gfx::SubresourceBuffer<Scene> buffer = {};
		Peio::Gfx::BufferSRV srv = {};

	};

}