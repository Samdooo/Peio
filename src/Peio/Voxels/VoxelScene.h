#pragma once

#include "VoxelsHeader.h"

namespace Peio::Vxl {

	struct VoxelScene {

		Uint2 screenSize;
		UINT numVoxels;
		UINT numMaterials;
		float voxelRadius;
		UINT numRays;

	};

}