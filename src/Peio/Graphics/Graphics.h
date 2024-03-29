#pragma once

#include "CommandQueue.h"
#include "CommandList.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Graphics {

		virtual void Clear(const Float4& color) = 0;
		virtual void Render() = 0;

		_NODISCARD ID3D12CommandQueue* GetCommandQueue() const noexcept;
		_NODISCARD ID3D12GraphicsCommandList* GetCommandList() const noexcept;
		_NODISCARD UINT GetFrameIndex() const noexcept;
		_NODISCARD const RenderTargetHeap& GetRenderTargets() const noexcept;
		_NODISCARD Long2 GetSize() const noexcept;

	protected:

		CommandQueue cmdQueue = {};
		CommandList cmdList = {};
		RenderTargetHeap renderTargets = {};
		Long2 size = {};

	};

}