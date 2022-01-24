#pragma once

#include "DescriptorHeap.h"
#include "Resource.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT UnorderedAccessView {

	protected:
		DescriptorHeap descriptorHeap;
		Resource* resources = nullptr;

	public:

		void Init(const std::vector<UINT64>& sizes, const std::vector<UINT64>& numElements, ID3D12GraphicsCommandList* cmdList);

		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD Resource* GetResources() const noexcept;
		_NODISCARD void** GetPointers() const noexcept;

	};

}