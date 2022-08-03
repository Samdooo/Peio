#pragma once

#include "ShaderResource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ConstantBufferView : public ShaderResource {

		void Init(UINT64 size, D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
		void Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) override;
		void SetAsRootParameter(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const override;

	protected:

		using Resource::Init;

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	};

}