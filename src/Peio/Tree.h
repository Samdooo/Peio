#pragma once

#include "Exception.h"

namespace Peio {

	template <typename T_branch, typename T_leaf>
	struct Tree {

	protected:

		size_t numLayers = 0;
		size_t numChildren = 0;

		T_branch** branches = nullptr;
		size_t numBranches = 0;

		T_leaf* leaves = nullptr;
		size_t numLeaves = 0;

	public:

		void Allocate(size_t numLayers, size_t numChildren) {
			this->numLayers = numLayers;
			this->numChildren = numChildren;
			if (numLayers > 1)
				branches = new T_branch * [numLayers - 1];
			numBranches = 0;
			numLeaves = 1;
			for (size_t i = 0; i < numLayers - 1; i++) {
				branches[i] = (T_branch*)numBranches;
				numBranches += numLeaves;
				numLeaves *= numChildren;
			}
			if (numBranches)
				branches[0] = new T_branch[numBranches];
			for (size_t i = 0; i < numLayers - 1; i++)
				branchLayers[i] = branches[0] + (size_t)branchLayers[i];
			if (numLeaves)
				leaves = new T_leaf[numLeaves];
		}

		_NODISCARD size_t GetNumLayers() const noexcept {
			return numLayers;
		}
		_NODISCARD size_t GetNumChildren() const noexcept {
			return numChildren;
		}

		_NODISCARD T_branch* const* GetBranches() const noexcept {
			return branchLayers;
		}
		_NODISCARD size_t GetNumBranches() const noexcept {
			return numBranches;
		}

		_NODISCARD T_leaf* GetLeaves() const noexcept {
			return leafBuffer;
		}
		_NODISCARD size_t GetNumLeaves() const noexcept {
			return numLeaves;
		}

		~PositionTree() {
			if (branches[0])
				delete[] branches[0];
			if (branches)
				delete[] branches;
			if (leaves)
				delete[] leaves;
		}

		struct Iterator {

		protected:

			const Tree& tree;
			size_t layerIndex;
			size_t index;

			Iterator(const Tree& tree, size_t layerIndex = 0, size_t index = 0) : tree(tree), layerIndex(layerIndex), index(index) {}

			_NODISCARD bool IsBranch() const noexcept {
				return layerIndex < tree.GetNumLayers() - 1;
			}
			_NODISCARD bool IsLeaf() const noexcept {
				return layerIndex == tree.GetNumLayers() - 1;
			}
			_NODISCARD bool HasParent() const noexcept {
				return layerIndex > 0;
			}

			_NODISCARD T_branch* GetBranch() const {
				if (IsLeaf()) {
					throw PEIO_EXCEPTION("Tried to get a branch from a leaf iterator.");
				}
				return &tree.GetBranches()[layerIndex][index];
			}
			_NODISCARD T_leaf* GetLeaf() const {
				if (IsBranch()) {
					throw PEIO_EXCEPTION("Tried to get a leaf from a branch iterator.");
				}
				return &tree.GetLeaves()[index];
			}

			_NODISCARD Iterator GetChild(size_t index) const {
				if (IsLeaf()) {
					throw PEIO_EXCEPTION("Tried to get a child from a leaf iterator.");
				}
				if (index >= tree.GetNumChildren()) {
					throw PEIO_EXCEPTION("Child index out of bounds.");
				}
				return Iterator(tree, layerIndex + 1, this->index * tree.GetNumChildren() + index);
			}

			_NODISCARD Iterator GetSibling(size_t sibling) const {
				if (layerIndex == 0) {
					throw PEIO_EXCEPTION("Tried to get a sibling from a root iterator.");
				}
				if (index >= tree.GetNumChildren()) {
					throw PEIO_EXCEPTION("Sibling index out of bounds.");
				}
				return Iterator(tree, layerIndex, this->index - (this->index % tree.GetNumChildren()) + index);
			}

			_NODISCARD Iterator GetParent() const {
				if (layerIndex == 0) {
					throw PEIO_EXCEPTION("Tried to get the parent of a root iterator.");
				}
				return Iterator(tree, layerIndex - 1, index / tree.GetNumChildren());
			}

		};

	};

}