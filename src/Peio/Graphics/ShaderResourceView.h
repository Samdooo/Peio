#pragma once

#include "Resource.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ShaderResourceView {

		void Init(UINT numResources);
		void CreateSRV(UINT index, D3D12_RESOURCE_DESC resourceDesc, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, D3D12_RESOURCE_STATES resourceState);
		void CreateBufferSRV(UINT index, UINT64 size, UINT64 numElements, D3D12_RESOURCE_STATES resourceState);
		void SetShaderVisibility(D3D12_SHADER_VISIBILITY visibility);

		_NODISCARD ID3D12DescriptorHeap** GetDescriptorHeaps() const noexcept;
		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD Resource* GetResources() noexcept;
		_NODISCARD UINT GetNumResources() const noexcept;
		_NODISCARD D3D12_SHADER_VISIBILITY GetVisibility() const noexcept;

		void Release();
		~ShaderResourceView();

	protected:

		ID3D12DescriptorHeap** descriptorHeaps = new ID3D12DescriptorHeap * [1];
		DescriptorHeap descriptorHeap = {};
		std::vector<Resource> resources = {};
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL;

	};

}