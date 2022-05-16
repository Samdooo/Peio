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

	void PositionTree::Remove(const Iterator& it) const
	{
		if (it.IsBranch()) {
			throw PEIO_EXCEPTION("Tried to remove a Branch.");
		}
		if (!it.HasParent())
			return;
		PositionBranch& branch = it.GetParent().GetBranch();
		branch.HasChild(it.GetIndex() % numChildren, false);
		branch.HasFull(it.GetIndex() % numChildren, false);
		if (it.GetParent().HasParent()) {
			UpdateDescriptors(it.GetParent().GetParent());
		}
		UpdateBoundaries(it.GetParent());
	}

	PositionTree::Ray PositionTree::TraceRay(const Float3& origin, const Float3& ray, UINT skip)
	{
		const uint numLayers = 10;
		const uint numChildren = 3;
		const Float3 invRay = Float3(1.0f, 1.0f, 1.0f) / ray;
		Float3 invRad = (invRay * 0.5f);
		invRad.x() = abs(invRad.x());
		invRad.y() = abs(invRad.y());
		invRad.z() = abs(invRad.z());

		Ray result(GetRootIterator());
		result.side = -1;
		result.normal = ray;

		float minScale = INFINITY;
		uint layerSize = numChildren; // In nodes
		uint layerOffset = 0;
		uint layerIndex = 0;
		uint nodeIndex = 0;

		uint branchIndices[numLayers];
		for (uint i = 0; i < numLayers; i++)
			branchIndices[i] = 0;

		uint descriptors[numLayers];
		descriptors[0] = -1;

		uint maxLayer = 0;
		bool up = true;
		uint checks = 0;

		while (true) {
			checks++;
			if (layerIndex > maxLayer)
				maxLayer = layerIndex;
			if (layerIndex == numLayers - 1) {
				for (uint v = 0; v < numChildren; v++) {
					if (!(descriptors[layerIndex] & (1U << (v * 2))))
						continue;
					Array<Float3, 2> boundaries = GetBoundaries(leaves[nodeIndex + v].index);
					boundaries[0] -= origin;
					boundaries[1] -= origin;

					boundaries[0] *= invRay;
					boundaries[1] *= invRay;

					float curMax = std::min(std::max(boundaries[0].x(), boundaries[1].x()),
						std::min(std::max(boundaries[0].y(), boundaries[1].y()),
							std::max(boundaries[0].z(), boundaries[1].z())));
					if (curMax <= 0.0f)
						continue;
					float curMin = std::max(std::min(boundaries[0].x(), boundaries[1].x()),
						std::max(std::min(boundaries[0].y(), boundaries[1].y()),
							std::min(boundaries[0].z(), boundaries[1].z())));

					if (curMax < curMin || curMin >= minScale)
						continue;

					minScale = curMin;
					result.collisionVoxel = leaves[nodeIndex + v].index;

					result.side = 0;
					for (int j = 1; j < 3; j++) {
						if (std::min(boundaries[0][j], boundaries[1][j]) > std::min(boundaries[0][result.side], boundaries[1][result.side]))
							result.side = j;
					}
					result.it = GetLeafIterator(nodeIndex + v);
				}
				up = false;
			}
			else {
				up = false;
				for (; branchIndices[layerIndex] < numChildren; branchIndices[layerIndex]++) {
					if (!(descriptors[layerIndex] & (1U << (branchIndices[layerIndex] * 2))))
						continue;
					PositionBranch branch = branches[0][layerOffset + nodeIndex + branchIndices[layerIndex]];

					branch.boundaries[0] -= origin;
					branch.boundaries[1] -= origin;

					branch.boundaries[0] *= invRay;
					branch.boundaries[1] *= invRay;

					float curMax = std::min(std::max(branch.boundaries[0].x(), branch.boundaries[1].x()),
						std::min(std::max(branch.boundaries[0].y(), branch.boundaries[1].y()),
							std::max(branch.boundaries[0].z(), branch.boundaries[1].z())));
					if (curMax <= 0.0f)
						continue;
					float curMin = std::max(std::min(branch.boundaries[0].x(), branch.boundaries[1].x()),
						std::max(std::min(branch.boundaries[0].y(), branch.boundaries[1].y()),
							std::min(branch.boundaries[0].z(), branch.boundaries[1].z())));

					if (curMax < curMin || curMin >= minScale)
						continue;

					nodeIndex += branchIndices[layerIndex];
					descriptors[layerIndex + 1] = branch.descriptor._Getword(0);
					branchIndices[layerIndex]++;
					branchIndices[layerIndex + 1] = 0;
					up = true;
					break;
				}
			}

			if (up) {
				layerIndex++;
				layerOffset += layerSize;
				layerSize *= numChildren;
				nodeIndex *= numChildren;
			}
			else {
				if (layerIndex == 0)
					break;
				layerIndex--;
				layerSize /= numChildren;
				layerOffset -= layerSize;
				nodeIndex /= numChildren * numChildren;
				nodeIndex *= numChildren;
			}
		}
		if (result.side != -1) {
			result.collision = origin + (ray * minScale);
			result.normal = ray;
			if (result.side == 0)
				result.normal.x() = -result.normal.x();
			else if (result.side == 1)
				result.normal.y() = -result.normal.y();
			else
				result.normal.z() = -result.normal.z();
		}
		return result;
	}


	//PositionTree::Iterator PositionTree::TraceRay(const Float3& origin, const Float3& ray, UINT skip)
	//{
	//	Iterator result = GetRootIterator();
	//	float minScale = D3D12_FLOAT32_MAX;
	//	for (size_t i = 0; i < numChildren; i++) {
	//		Iterator it = GetRootIterator(i);
	//		TraceRay(origin, ray, skip, it, minScale, result);
	//	}
	//	return result;
	//}
	//
	//void PositionTree::TraceRay(const Float3& origin, const Float3& ray, UINT skip, Iterator it, float& minScale, Iterator& result)
	//{
	//	Array<Float3, 2> boundaries = it.IsLeaf() ? GetBoundaries(it.GetLeaf().index) : it.GetBranch().boundaries;
	//	boundaries[0] -= origin;
	//	boundaries[1] -= origin;
	//	boundaries[0] /= ray;
	//	boundaries[1] /= ray;
	//
	//	float curMax = std::min(std::max(boundaries[0].x(), boundaries[1].x()),
	//		std::min(std::max(boundaries[0].y(), boundaries[1].y()),
	//			std::max(boundaries[0].z(), boundaries[1].z())));
	//
	//	if (curMax <= 0.0f)
	//		return;
	//
	//	float curMin = std::max(std::min(boundaries[0].x(), boundaries[1].x()),
	//		std::max(std::min(boundaries[0].y(), boundaries[1].y()),
	//			std::min(boundaries[0].z(), boundaries[1].z())));
	//
	//	if (curMin > curMax || curMin >= minScale)
	//		return;
	//
	//	if (it.IsLeaf()) {
	//		result = it;
	//		minScale = curMin;
	//	}
	//	else {
	//		for (size_t i = 0; i < numChildren; i++) {
	//			if (it.GetBranch().HasChild(i)) {
	//				TraceRay(origin, ray, skip, it.GetChild(i), minScale, result);
	//			}
	//		}
	//	}
	//}

}