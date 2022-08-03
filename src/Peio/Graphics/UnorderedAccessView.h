#pragma once

#include "ShaderResource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT UnorderedAccessView : public ShaderResource {

		void Init(D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, bool copyFootprints = false);
		void Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) override;
		void SetAsRootParameter(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const override;

	protected:

		using Resource::Init;

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	};

	struct PEIO_GFX_EXPORT BufferUAV : public UnorderedAccessView {

		void Init(UINT64 size, UINT numElements, D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);

	protected:

		using UnorderedAccessView::Init;

	};

}