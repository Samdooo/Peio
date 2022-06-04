#pragma once

#include "ResourceArray.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT RootSignature {

		static ID3D12RootSignature* Create(const std::vector<D3D12_ROOT_PARAMETER>& params
			, const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers = std::vector<D3D12_STATIC_SAMPLER_DESC>()
			, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	};

}