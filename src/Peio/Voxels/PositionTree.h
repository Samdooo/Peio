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

		struct Ray {
			Iterator it;
			UINT collisionVoxel = -1;
			int side = -1; // -1 indicates no collision
			Float3 collision = {};
			Float3 normal = {};
			Ray(const Iterator& it) : it(it) {}
		};

		virtual Array<Float3, 2> GetBoundaries(UINT) const = 0;

		bool Insert(PositionLeaf leaf) const;
		void Remove(const Iterator& it) const;

		Ray TraceRay(const Float3& origin, const Float3& ray, UINT skip = -1);
		//Iterator TraceRay(const Float3& origin, const Float3& ray, UINT skip);

		void UpdateDescriptors(const Iterator& it) const;
		void UpdateBoundaries(const Iterator& it) const;

	protected:

		bool Insert(PositionLeaf leaf, const Iterator& it, size_t index) const;
		//void TraceRay(const Float3& origin, const Float3& ray, UINT skip, Iterator it, float& minScale, Iterator& result);

	};

}