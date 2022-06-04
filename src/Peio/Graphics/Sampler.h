#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Sampler {

		static D3D12_STATIC_SAMPLER_DESC Create(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_PIXEL, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_BORDER);

	};

}