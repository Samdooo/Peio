#pragma once

#include "VoxelsHeader.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT Material {
		Peio::Float4 reflection = {};
		Peio::Float3 emission = {};
		float spread = 0.0f;
	};

}