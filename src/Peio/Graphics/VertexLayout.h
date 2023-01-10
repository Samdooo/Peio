#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT VertexElement {
		std::string name = {};
		DXGI_FORMAT format = {};
	};

	struct PEIO_GFX_EXPORT VertexLayout {

		std::vector<VertexElement> elements = {};

		_NODISCARD D3D12_INPUT_LAYOUT_DESC CreateLayout();

	protected:

		D3D12_INPUT_LAYOUT_DESC layoutDesc = {};
		std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};

	};

}