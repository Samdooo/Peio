#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT DescriptorRange {

		static D3D12_DESCRIPTOR_RANGE Create(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT n_descriptors, UINT index);

	};

	struct PEIO_GFX_EXPORT DescriptorTable {

		static D3D12_ROOT_DESCRIPTOR_TABLE Create(const std::vector<D3D12_DESCRIPTOR_RANGE>& ranges);

	};

}