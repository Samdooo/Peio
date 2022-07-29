#pragma once

#include "IndexMap.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT MaterialMap : public IndexMap<uint, 3> {

		struct PEIO_VXL_EXPORT Ray {
			Uint3 voxel = {};
			uint side = ~0;
			Double3 normal = {};
			uint material = ~0;
			Double3 collision = {};
		};

		_NODISCARD Ray Trace(Double3 origin, Double3 ray, Uint3 skip = Uint3(-1, -1, -1));

	};

	//struct PEIO_VXL_EXPORT MaterialMap {
	//
	//	struct PEIO_VXL_EXPORT Group {
	//		uint indices[2][2][2] = {
	//			{ { ~0U, ~0U }, { ~0U, ~0U } },
	//			{ { ~0U, ~0U }, { ~0U, ~0U } }
	//		};
	//		_NODISCARD bool operator==(const Group& that) const;
	//	};
	//
	//	struct PEIO_VXL_EXPORT Ray {
	//		Uint3 voxel = {};
	//		uint side = ~0;
	//		Double3 normal = {};
	//		uint material = ~0;
	//		Double3 collision = {};
	//	};
	//
	//	void Init(uint maxSize, uint numLayers);
	//
	//	_NODISCARD uint GetMaterial(Uint3 pos) const;
	//	void SetMaterial(Uint3 pos, uint material, bool log = false);
	//
	//	_NODISCARD Group* GetGroups();
	//	_NODISCARD uint GetNumGroups();
	//
	//	_NODISCARD Ray Trace(Double3 origin, Double3 ray, Uint3 skip = Uint3(-1, -1, -1));
	//
	////protected:
	//
	//	_NODISCARD uint GetNext();
	//	void AddDeleted(uint index);
	//	void RemoveRef(uint index);
	//	
	//	struct PEIO_VXL_EXPORT GroupHash {
	//		_NODISCARD size_t operator()(const Group& group) const;
	//	};
	//
	//	uint numLayers = 0;
	//	std::vector<Group> groups = {};
	//	std::unordered_map<Group, uint, GroupHash> map = {};
	//	std::vector<size_t> references = {};
	//	std::vector<uint> deleted = {};
	//	uint numDeleted = 0;
	//	uint nextFree = 0;
	//
	//	std::vector<uint> path = {};
	//
	//};

}