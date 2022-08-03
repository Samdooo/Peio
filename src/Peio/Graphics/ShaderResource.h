#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ShaderResource : public Resource {

		virtual void Create(D3D12_CPU_DESCRIPTOR_HANDLE) = 0;
		virtual void SetAsRootParameter(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const = 0;

	};

}