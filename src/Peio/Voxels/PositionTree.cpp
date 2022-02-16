#define PEIO_VXL_EXPORTING
#include "PositionTree.h"

namespace Peio::Vxl {

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
			if (that[0][i] > boundaries[1][i]) {
				boundaries[0][i] = that[1][i];
				changed = true;
			}
		}
		return changed;
	}

	bool PositionBranch::Contains(const Array<Float3, 2>& that) const
	{
		for (size_t i = 0; i < 3; i++)
			if (that[0][i] > boundaries[1][i] || that[1][i] < boundaries[0][i])
				return false;
		return true;
	}

	void PositionTree::UpdateParent(const Iterator& it) const
	{
		if (!it.HasParent())
			return;
		if (it.GetParent().GetBranch().Fit(it.GetBranch().boundaries))
			UpdateParent(it.GetParent());
	}

}