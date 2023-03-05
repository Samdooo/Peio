#pragma once

#include "FullscreenRenderer.h"

namespace Renderer {

	struct Denoiser : public FullscreenRenderer {

		UINT numDims = 0;

		void Init();

	};

}