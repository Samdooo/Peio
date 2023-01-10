#pragma once

#include "Descriptor.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT VertexBuffer : public ArrayBufferResource {

		_NODISCARD const D3D12_VERTEX_BUFFER_VIEW& GetView();

	protected:

		D3D12_VERTEX_BUFFER_VIEW view = {};

	};

}