#pragma once

#include "Global.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT DescriptorHeap {

	protected:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap = nullptr;
		UINT handleIncrementSize = 0;

		void CheckHeap() const;

	public:

		void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors = 1, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		_NODISCARD ID3D12DescriptorHeap* GetHeap() const noexcept;
		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const;
		_NODISCARD D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const;
		_NODISCARD UINT GetHandleIncrementSize() const noexcept;

		void Release();

		~DescriptorHeap();

	};

}