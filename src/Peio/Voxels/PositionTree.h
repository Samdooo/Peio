#pragma once

#include "VoxelsHeader.h"
#include "..\Tree.h"
#include <bitset>

namespace Peio::Vxl {

	struct PositionBranch {
		std::bitset<32> descriptor = {};
		Array<Float3, 2> boundaries = {};

		bool HasChild(size_t index) const;
		void HasChild(size_t index, bool set);
		bool HasFull(size_t index) const;
		void HasFull(size_t index, bool set);
		bool IsFull(size_t numChildren) const;
		bool Intersects(const Array<Float3, 2>& that) const;
	};
	struct PositionLeaf {
		UINT index;
	};

	struct PEIO_VXL_EXPORT PositionTree : public Tree<PositionBranch, PositionLeaf> {
		
		using Iterator = Tree<PositionBranch, PositionLeaf>::Iterator;

		virtual Array<Float3, 2> GetBoundaries(UINT) const = 0;

		bool Insert(PositionLeaf leaf) const;
		void UpdateDescriptors(const Iterator& it) const;
		void UpdateBoundaries(const Iterator& it) const;

	protected:

		bool Insert(PositionLeaf leaf, const Iterator& it, size_t index) const;

	};

}