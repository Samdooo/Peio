#pragma once

#include "CommandAllocator.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT CommandList {

	protected:

		CommandAllocator** allocators = nullptr;
		UINT numBuffers = 0, numAllocators = 0;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;

		ID3D12CommandList* cmdLists[1] = {};

	public:

		void Init(ID3D12CommandQueue* cmdQueue, UINT numBuffers, UINT numAllocators);

		void Reset(UINT allocator, UINT frameIndex);
		void Close();

		_NODISCARD CommandAllocator& GetAllocator(UINT allocator, UINT frameIndex) const noexcept;

		_NODISCARD ID3D12GraphicsCommandList* GetCommandList() const noexcept;
		_NODISCARD ID3D12CommandList* const* GetCommandLists() const noexcept;

		void Release();

		~CommandList();

	};

}