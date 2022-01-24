#pragma once

#include "Global.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT CommandQueue {

	protected:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;

	public:

		void Init();
		void Execute(ID3D12CommandList* const* lists, UINT numLists);

		_NODISCARD ID3D12CommandQueue* GetQueue() const noexcept;

		void Release();

		~CommandQueue();

	};

}