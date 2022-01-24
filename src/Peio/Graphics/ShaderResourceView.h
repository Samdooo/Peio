#pragma once

#include "Resource.h"
#include "DescriptorHeap.h"


namespace Peio::Gfx {

	class PEIO_GFX_EXPORT ShaderResourceView {

	protected:

		ID3D12DescriptorHeap** descriptorHeaps = new ID3D12DescriptorHeap * [1];
		DescriptorHeap descriptorHeap = {};
		Resource* resources = nullptr;
		UINT numResources = 0;

	public:

		void Init(const std::vector<D3D12_RESOURCE_DESC>& descs, const std::vector<D3D12_RESOURCE_STATES>& states, const std::vector<D3D12_SHADER_RESOURCE_VIEW_DESC>& srvDescs);

		void InitBuffer(const std::vector<UINT64>& sizes, const std::vector<UINT>& numElements, const std::vector<D3D12_RESOURCE_STATES>& states);

		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD ID3D12DescriptorHeap** GetDescriptorHeaps() const noexcept;
		_NODISCARD Resource* GetResources() const noexcept;
		_NODISCARD UINT GetNumResources() const noexcept;

		void Release();

		~ShaderResourceView();

	};

}