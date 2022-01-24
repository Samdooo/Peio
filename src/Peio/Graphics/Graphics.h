#pragma once

#include "CommandQueue.h"
#include "CommandList.h"
#include "RenderTargetHeap.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT Graphics {

	protected:

		CommandQueue cmdQueue = {};
		CommandList cmdList = {};
		RenderTargetHeap renderTargets = {};

	public:

		virtual void Clear(const Float4& color) = 0;
		virtual void Render() = 0;

		_NODISCARD ID3D12CommandQueue* GetCommandQueue() const noexcept;
		_NODISCARD ID3D12GraphicsCommandList* GetCommandList() const noexcept;
		_NODISCARD UINT GetFrameIndex() const noexcept;

	};

}