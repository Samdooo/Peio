#pragma once

#include "VoxelsHeader.h"

namespace Peio::Vxl {

	struct VoxelScene {

		UINT numVoxels;
		UINT numMaterials;
		float voxelRadius;
		UINT maxNumRays;
		UINT maxHitRays;
		float minLightDeviation;
		UINT useScene = 1;
		//UINT numCollisions;
		//float lightRadius;

	};

}