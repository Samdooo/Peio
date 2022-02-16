#define PEIO_VXL_EXPORTING
#include "PositionTree.h"

namespace Peio::Vxl {

	bool PositionBranch::IsFull(UINT numChildren) const
	{
		return descriptor == ((1 << numChildren) - 1);
	}

	bool PositionBranch::GetStatus(UINT index) const
	{
		return descriptor & (1 << index);
	}

	void PositionBranch::SetStatus(UINT index, bool full)
	{
		if (full)
			descriptor |= (1 << index);
		else
			descriptor &= ~(1 << index);
	}

	bool PositionBranch::Fit(const Array<Float3, 2>& that)
	{
		bool changed = false;
		for (size_t i = 0; i < 3; i++) {
			if (that[0][i] < boundaries[0][i]) {
				boundaries[0][i] = that[0][i];
				changed = true;
			}
		}
		for (size_t i = 0; i < 3; i++) {
			if (that[1][i] > boundaries[1][i]) {
				boundaries[1][i] = that[1][i];
				changed = true;
			}
		}
		return changed;
	}

	bool PositionBranch::Intersects(const Array<Float3, 2>& that) const
	{
		for (size_t i = 0; i < 3; i++)
			if (that[0][i] > boundaries[1][i] || that[1][i] < boundaries[0][i])
				return false;
		return true;
	}

	bool PositionTree::Insert(PositionLeaf leaf) const
	{
		for (size_t i = 0; i < numChildren; i++)
			if (Insert(leaf, GetRootIterator(i)))
				return true;
		return false;
	}

	void PositionTree::UpdateBoundaries(const Iterator& it) const
	{
		if (!it.HasParent())
			return;
		Array<Float3, 2> boundaries;
		if (it.IsBranch())
			boundaries = it.GetBranch().boundaries;
		else
			boundaries = GetBoundaries(it.GetLeaf().index);
		if (it.GetParent().GetBranch().Fit(boundaries)) {
			UpdateBoundaries(it.GetParent());
		}
	}

	bool PositionTree::Insert(PositionLeaf leaf, const Iterator& it) const
	{
		if (it.IsBranch()) {
			for (UINT i = 0; i < numChildren; i++) {
				if (!it.GetBranch().GetStatus(i)) {
					Insert(leaf, it.GetChild(i));
					if (it.GetChild(i).IsLeaf() || it.GetChild(i).GetBranch().IsFull(numChildren))
						it.GetBranch().SetStatus(i, true);
					return true;
				}
			}
			return false;
		}
		else {
			it.GetLeaf() = leaf;
			UpdateBoundaries(it);
			return true;
		}
	}

}