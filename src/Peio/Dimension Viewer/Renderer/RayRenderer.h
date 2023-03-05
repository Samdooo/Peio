#pragma once

#include "FullscreenRenderer.h"

namespace Renderer {

	struct RayRenderer : public FullscreenRenderer {

		UINT numDims = 0;
		UINT maxBounces = 0;

		void Init();

	};

}