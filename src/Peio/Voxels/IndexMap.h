#pragma once

#include "VoxelsHeader.h"
#include "..\Exception.h"
#include <functional>
#include <vector>
#include <unordered_map>

namespace Peio::Vxl {

	template <typename T_index, T_index numDimensions>
	struct IndexMap {

		static constexpr T_index null = std::numeric_limits<T_index>::max();

		using Position = Array<T_index, numDimensions>;

		template <T_index dim>
		struct NodeDimension {
			NodeDimension<dim - 1> indices[2] = { NodeDimension<dim - 1>(), NodeDimension<dim - 1>() };
			_NODISCARD NodeDimension<dim - 1>& operator[](bool i) {
				return indices[i];
			}
			_NODISCARD const NodeDimension<dim - 1>& operator[](bool i) const {
				return indices[i];
			}
			_NODISCARD size_t GetHash() const {
				return indices[0].GetHash() ^ indices[1].GetHash();
			}
			_NODISCARD bool operator==(const NodeDimension<dim>& that) const {
				return indices[0] == that[0] && indices[1] == that[1];
			}
			_NODISCARD T_index& AtLayer(const Position& position, T_index layer, T_index numLayers) {
				return indices[(bool)(position[numDimensions - dim] & ((T_index)1 << (numLayers - 1 - layer)))].AtLayer(position, layer, numLayers);
			}
			_NODISCARD const T_index& AtLayer(const Position& position, T_index layer, T_index numLayers) const {
				return indices[(bool)(position[numDimensions - dim] & ((T_index)1 << (numLayers - 1 - layer)))].AtLayer(position, layer, numLayers);
			}
		};
		template <>
		struct NodeDimension<1> {
			T_index indices[2] = { null, null };
			_NODISCARD T_index& operator[](bool i) {
				return indices[i];
			}
			_NODISCARD const T_index& operator[](bool i) const {
				return indices[i];
			}
			_NODISCARD size_t GetHash() const {
				return std::hash<T_index>()(indices[0]) ^ std::hash<T_index>()(indices[1]); // Can probably be improved
			}
			_NODISCARD bool operator==(const NodeDimension<1>& that) const {
				return indices[0] == that[0] && indices[1] == that[1];
			}
			_NODISCARD T_index& AtLayer(const Position& position, T_index layer, T_index numLayers) {
				return indices[(bool)(position[numDimensions - 1] & ((T_index)1 << (numLayers - 1 - layer)))];
			}
			_NODISCARD const T_index& AtLayer(const Position& position, T_index layer, T_index numLayers) const {
				return indices[(bool)(position[numDimensions - 1] & ((T_index)1 << (numLayers - 1 - layer)))];
			}
		};
		using Node = NodeDimension<numDimensions>;
		struct NodeHash {
			size_t operator()(const Node& node) const {
				return node.GetHash();
			}
		};

	//protected:

		_NODISCARD T_index NewNode() {
			if (numDeleted) {
				return deleted[--numDeleted];
			}
			else {
				if (nextFree == nodes.size())
					throw PEIO_EXCEPTION("IndexMap overflow.");
				return nextFree++;
			}
		}

		void RemoveReference(T_index index, T_index layer) {
			if (references[index] == 0)
				return;
			references[index]--;
			if (references[index] == 0) {
				deleted[numDeleted++] = index;
				if (layer == numLayers - 1)
					leafMap.erase(nodes[index]);
				else
					branchMap.erase(nodes[index]);
				if (layer < numLayers - 1) {
					for (T_index i = 0; i < ((T_index)1 << numDimensions); i++) {
						T_index child = ((T_index*)&nodes[index])[i];
						if (child != null)
							RemoveReference(child, layer + 1);
					}
				}
				nodes[index] = Node();
			}
		}

		T_index numLayers = 0;
		std::vector<Node> nodes = {};
		std::unordered_map<Node, T_index, NodeHash> branchMap = {}, leafMap = {};
		std::vector<size_t> references = {};
		std::vector<T_index> deleted = {};
		size_t numDeleted = 0;
		T_index nextFree = 1;

		std::vector<T_index> path = {};

	public:

		void Init(T_index numLayers, size_t numNodes) {
			this->numLayers = numLayers;
			nodes.resize(numNodes, Node());
			branchMap.reserve(numNodes); // Probably change sizes later
			leafMap.reserve(numNodes);
			references.resize(numNodes);
			deleted.resize(numNodes);
			numDeleted = 0;
			nextFree = 1;
			path.resize(numLayers);
		}

		_NODISCARD T_index GetIndex(const Position& position, T_index layer = ~0) const {
			if (layer > numLayers - 1)
				layer = numLayers - 1;
			T_index cur = 0;
			for (T_index l = 0; l <= layer; l++) {
				cur = nodes[cur].AtLayer(position, l, numLayers);
				if (cur == null)
					break;
			}
			return cur;
		}

		void SetIndex(const Position& position, T_index newIndex, T_index layer = null) {
			if (layer > numLayers - 1)
				layer = numLayers - 1;

			path[0] = 0;
			T_index firstRef = null;
			for (T_index l = 0; l + 1 <= layer; l++) {
				if (path[l] == null)
					path[l + 1] = null;
				else
					path[l + 1] = nodes[path[l]].AtLayer(position, l, numLayers);
				if (firstRef == null && path[l + 1] != null && references[path[l + 1]] > 1)
					firstRef = l + 1;
			}

			for (T_index l = layer;; l--) {
				Node node = (path[l] == null) ? Node() : nodes[path[l]];
				T_index& ind = node.AtLayer(position, l, numLayers);
				if (ind == newIndex)
					break;
				T_index old = ind;
				ind = newIndex;

				if (l == 0) {
					if (old != null && l < numLayers - 1)
						RemoveReference(old, l + 1);
					if (newIndex != null && l < numLayers - 1)
						references[newIndex]++;
					nodes[0] = node;
					break;
				}

				if (node == Node()) { // Deletion
					newIndex = null;
				}
				else if ((l == numLayers - 1) ? leafMap.contains(node) : branchMap.contains(node)) { // Compression
					if (newIndex != null && l < numLayers - 1)
						references[newIndex]++;
					newIndex = (l == numLayers - 1) ? leafMap.at(node) : branchMap.at(node);
				}
				else if (path[l] == null || l >= firstRef) { // Copying
					if (l < numLayers - 1) {
						for (T_index i = 0; i < ((T_index)1 << numDimensions); i++) {
							T_index child = ((T_index*)&node)[i];
							if (child != null)
								references[child]++;
						}
					}
					newIndex = NewNode();
					nodes[newIndex] = node;
					if (l == numLayers - 1)
						leafMap.insert({ node, newIndex });
					else
						branchMap.insert({ node, newIndex });
				}
				else { // Overwriting
					if (old != null && l < numLayers - 1)
						RemoveReference(old, l + 1);
					if (newIndex != null && l < numLayers - 1)
						references[newIndex]++;
					if (l == numLayers - 1)
						leafMap.erase(nodes[path[l]]);
					else
						branchMap.erase(nodes[path[l]]);
					nodes[path[l]] = node;
					newIndex = path[l];
					if (l == numLayers - 1)
						leafMap.insert({ node, newIndex });
					else
						branchMap.insert({ node, newIndex });
				}
			}
		}

	};

}