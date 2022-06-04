#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ConstantBufferView : public Resource {

		void Init(UINT64 size, D3D12_RESOURCE_STATES resourceState);
		void Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) override;

	protected:

		using Resource::Init;

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	};

}