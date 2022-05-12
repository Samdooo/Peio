#pragma once

#include "Resource.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT UnorderedAccessView {

		void Init(const std::vector<D3D12_RESOURCE_DESC>& descs, const std::vector<D3D12_RESOURCE_STATES>& states, const std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC>& uavDescs);
		void InitBuffer(const std::vector<UINT64>& sizes, const std::vector<UINT>& numElements, const std::vector<D3D12_RESOURCE_STATES>& states);
		void InitTexture2D(const std::vector<Uint2>& sizes, const std::vector<DXGI_FORMAT>& formats, const std::vector<D3D12_RESOURCE_STATES>& states);

		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD ID3D12DescriptorHeap** GetDescriptorHeaps() const noexcept;
		_NODISCARD Resource* GetResources() const noexcept;
		_NODISCARD UINT GetNumResources() const noexcept;

		void Release();

		~UnorderedAccessView();

	protected:

		ID3D12DescriptorHeap** descriptorHeaps = new ID3D12DescriptorHeap * [1];
		DescriptorHeap descriptorHeap = {};
		Resource* resources = nullptr;
		UINT numResources = 0;

	};

}