#pragma once

#include "DescriptorHeap.h"
#include "Heap.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT ConstantBufferView {

	protected:
		DescriptorHeap descriptorHeap;
		Heap* heaps = nullptr;
		void** ptrs = nullptr;

	public:

		void Init(const std::vector<UINT64>& sizes);

		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD Heap* GetHeaps() const noexcept;
		_NODISCARD void** GetPointers() const noexcept;

	};

}