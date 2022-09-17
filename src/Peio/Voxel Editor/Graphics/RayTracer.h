#pragma once

#include "..\..\Voxels\FullscreenRenderer.h"

namespace Graphics {

	struct RayTracer {

		void Init();
		void Render();

		Peio::Vxl::FullscreenRenderer renderer = {};

	};

}