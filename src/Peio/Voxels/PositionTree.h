#pragma once

#include "VoxelsHeader.h"
#include "..\Tree.h"

namespace Peio::Vxl {

	struct PositionBranch {
		UINT descriptor;
		Array<Float3, 2> boundaries;

		bool Fit(const Array<Float3, 2>& that);
		bool Contains(const Array<Float3, 2>& that) const;
	};
	struct PositionLeaf {
		UINT voxelIndex;
	};

	struct PEIO_VXL_EXPORT PositionTree : public Tree<PositionBranch, PositionLeaf> {
		
		using Iterator = Tree<PositionBranch, PositionLeaf>::Iterator;

		void UpdateParent(const Iterator& it) const;

	};

}