#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT UnorderedAccessView : public Resource {

		void Init(D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false);
		void Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) override;

	protected:

		using Resource::Init;

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	};

	struct PEIO_GFX_EXPORT BufferUAV : public UnorderedAccessView {

		void Init(UINT64 size, UINT numElements, D3D12_RESOURCE_STATES resourceState);

	protected:

		using UnorderedAccessView::Init;

	};

}