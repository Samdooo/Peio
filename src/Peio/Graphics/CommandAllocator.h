#pragma once

#include "Global.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT CommandAllocator {

	protected:

		ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
		ComPtr<ID3D12Fence> fence = nullptr;

		UINT64 fenceTarget = 0;
		HANDLE fenceEvent = nullptr;

	public:

		void Init();

		void Signal(ID3D12CommandQueue* cmdQueue);
		void Wait() const;
		void Reset() const;

		_NODISCARD ID3D12CommandAllocator* GetAllocator() const noexcept;
		_NODISCARD ID3D12Fence* GetFence() const noexcept;

		~CommandAllocator();

	};

}