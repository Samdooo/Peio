#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ShaderResourceBuffer : public Resource {

		void CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc);
		void CreateSRVBuffer(UINT64 size);

	};

}