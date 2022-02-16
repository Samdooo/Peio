#pragma once

#include "VoxelsHeader.h"
#include "..\Tree.h"

namespace Peio::Vxl {

	struct PositionBranch {
		UINT descriptor = 0;
		Array<Float3, 2> boundaries = {};

		bool IsFull(UINT numChildren) const;
		bool GetStatus(UINT index) const;
		void SetStatus(UINT index, bool full);
		bool Fit(const Array<Float3, 2>& that);
		bool Intersects(const Array<Float3, 2>& that) const;
	};
	struct PositionLeaf {
		UINT index;
	};

	struct PEIO_VXL_EXPORT PositionTree : public Tree<PositionBranch, PositionLeaf> {
		
		using Iterator = Tree<PositionBranch, PositionLeaf>::Iterator;

		virtual Array<Float3, 2> GetBoundaries(UINT) const = 0;

		bool Insert(PositionLeaf leaf) const;

	protected:

		bool Insert(PositionLeaf leaf, const Iterator& it) const;
		void UpdateBoundaries(const Iterator& it) const;

	};

}