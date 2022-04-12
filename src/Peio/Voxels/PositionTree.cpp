#define PEIO_VXL_EXPORTING
#include "PositionTree.h"

namespace Peio::Vxl {

	bool PositionBranch::HasChild(size_t index) const
	{
		return descriptor.test(index * 2);
	}

	void PositionBranch::HasChild(size_t index, bool set)
	{
		descriptor.set(index * 2, set);
	}

	bool PositionBranch::HasFull(size_t index) const
	{
		return descriptor.test(index * 2 + 1);
	}

	void PositionBranch::HasFull(size_t index, bool set)
	{
		descriptor.set(index * 2 + 1, set);
	}

	bool PositionBranch::IsFull(size_t numChildren) const
	{
		return descriptor._Getword(0) == ((1U << (numChildren * 2)) - 1U);
	}

	bool PositionBranch::Intersects(const Array<Float3, 2>& that) const
	{
		for (size_t i = 0; i < 3; i++)
			if (that[0][i] > boundaries[1][i] || that[1][i] < boundaries[0][i])
				return false;
		return true;
	}

	void PositionTree::UpdateDescriptors(const Iterator& it) const
	{
		if (it.IsLeaf())
			throw PEIO_EXCEPTION("UpdateDescriptors called on a Leaf iterator.");

		PositionBranch branch;
		for (size_t i = 0; i < numChildren; i++) {
			Iterator child = it.GetChild(i);
			if (!child.IsLeaf()) {
				branch.HasChild(i, child.GetBranch().descriptor.any());
				branch.HasFull(i, child.GetBranch().IsFull(numChildren));
			}
		}
		if (branch.descriptor != it.GetBranch().descriptor) {
			it.GetBranch().descriptor = branch.descriptor;
			
		}
		if (it.HasParent())
			UpdateDescriptors(it.GetParent());
	}

	void PositionTree::UpdateBoundaries(const Iterator& it) const
	{
		if (it.IsLeaf())
			throw PEIO_EXCEPTION("UpdateBoundaries called on a Leaf iterator.");

		PositionBranch& branch = it.GetBranch();
		Array<Float3, 2> boundaries = {};

		for (size_t i = 0; i < numChildren; i++) {
			if (!branch.HasChild(i))
				continue;
			Iterator child = it.GetChild(i);
			Array<Float3, 2> childBounds;
			if (child.IsLeaf())
				childBounds = GetBoundaries(child.GetLeaf().index);
			else
				childBounds = child.GetBranch().boundaries;
			if (i == 0)
				boundaries = childBounds;
			else {
				for (size_t j = 0; j < 3; j++) {
					if (childBounds[0][j] < boundaries[0][j])
						boundaries[0][j] = childBounds[0][j];
					if (childBounds[1][j] > boundaries[1][j])
						boundaries[1][j] = childBounds[1][j];
				}
			}
		}
		if (branch.boundaries != boundaries) {
			branch.boundaries = boundaries;
			if (it.HasParent())
				UpdateBoundaries(it.GetParent());
		}
	}

	bool PositionTree::Insert(PositionLeaf leaf) const
	{
		for (size_t i = 0; i < numChildren; i++)
			if (Insert(leaf, GetRootIterator(i), i))
				return true;
		return false;
	}

	bool PositionTree::Insert(PositionLeaf leaf, const Iterator& it, size_t index) const
	{
		if (it.IsBranch()) {
			PositionBranch& branch = it.GetBranch();

			for (size_t i = 0; i < numChildren; i++) {
				if (!branch.HasFull(i)) {
					return Insert(leaf, it.GetChild(i), i);
					//branch.HasChild(i, true);
					//if (it.GetChild(i).IsLeaf() || it.GetChild(i).GetBranch().IsFull(numChildren))
					//	it.GetBranch().HasFull(i, true);
					return true;
				}
			}
			return false;
		}
		else {
			it.GetLeaf() = leaf;
			if (it.HasParent()) {
				Iterator parent = it.GetParent();
				parent.GetBranch().HasChild(index, true);
				parent.GetBranch().HasFull(index, true);
				if (parent.HasParent())
					UpdateDescriptors(parent.GetParent());
				UpdateBoundaries(parent);
			}
			return true;
		}
	}

}