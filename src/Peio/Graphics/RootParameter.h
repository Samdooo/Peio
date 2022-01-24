#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT RootParameter {

		static D3D12_ROOT_PARAMETER CreateConstantBufferView(UINT index, D3D12_SHADER_VISIBILITY visibility);
		static D3D12_ROOT_PARAMETER CreateShaderResourceView(UINT index, D3D12_SHADER_VISIBILITY visibility);
		static D3D12_ROOT_PARAMETER CreateDescriptorTable(D3D12_ROOT_DESCRIPTOR_TABLE table, D3D12_SHADER_VISIBILITY visibility);

	};

}