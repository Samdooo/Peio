#pragma once

#include "Global.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT InputElement {

		static D3D12_INPUT_ELEMENT_DESC Create(LPCSTR name, DXGI_FORMAT format, UINT byteOffset = 0
			, D3D12_INPUT_CLASSIFICATION inputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, UINT semanticIndex = 0, UINT inputSlot = 0, UINT instanceDataStepRate = 0);

	};

	struct PEIO_GFX_EXPORT InputLayout {

		static D3D12_INPUT_LAYOUT_DESC Create(std::vector<D3D12_INPUT_ELEMENT_DESC> elements);

	};

}