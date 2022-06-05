#define PEIO_GFX_EXPORTING
#include "Graphics.h"

namespace Peio::Gfx {

	ID3D12CommandQueue* Graphics::GetCommandQueue() const noexcept
	{
		return cmdQueue.GetQueue();
	}

	ID3D12GraphicsCommandList* Graphics::GetCommandList() const noexcept
	{
		return cmdList.GetCommandList();
	}

	UINT Graphics::GetFrameIndex() const noexcept
	{
		return renderTargets.GetFrameIndex();
	}

	const RenderTargetHeap& Graphics::GetRenderTargets() const noexcept
	{
		return renderTargets;
	}

	Long2 Graphics::GetSize() const noexcept
	{
		return size;
	}

}