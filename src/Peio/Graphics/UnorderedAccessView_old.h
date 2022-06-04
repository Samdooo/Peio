#pragma once

#include "Resource.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT UnorderedAccessView {

		void Init(UINT numResources);
		void CreateUAV(UINT index, D3D12_RESOURCE_DESC resourceDesc, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_STATES resourceState);
		void CreateBufferUAV(UINT index, UINT64 size, UINT64 numElements, D3D12_RESOURCE_STATES resourceState);
		void SetShaderVisibility(D3D12_SHADER_VISIBILITY visibility);

		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD ID3D12DescriptorHeap** GetDescriptorHeaps() const noexcept;
		_NODISCARD Resource** GetResources() noexcept;
		_NODISCARD UINT GetNumResources() const noexcept;
		_NODISCARD D3D12_SHADER_VISIBILITY GetVisibility() const noexcept;


		void Release();
		~UnorderedAccessView();

	protected:

		ID3D12DescriptorHeap** descriptorHeaps = new ID3D12DescriptorHeap * [1];
		DescriptorHeap descriptorHeap = {};
		std::vector<Resource*> resources = {};
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL;

	};

}