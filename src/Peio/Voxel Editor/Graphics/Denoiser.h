#pragma once

#include "..\..\Voxels\FullscreenRenderer.h"

namespace Graphics {

	struct Denoiser {

		void Init();
		void Render();

		Peio::Vxl::FullscreenRenderer renderer = {};

	};

}