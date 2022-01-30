#define PEIO_GFX_EXPORTING
#include "Graphics.h"

ID3D12CommandQueue* Peio::Gfx::Graphics::GetCommandQueue() const noexcept
{
	return cmdQueue.GetQueue();
}

ID3D12GraphicsCommandList* Peio::Gfx::Graphics::GetCommandList() const noexcept
{
	return cmdList.GetCommandList();
}

UINT Peio::Gfx::Graphics::GetFrameIndex() const noexcept
{
	return renderTargets.GetFrameIndex();
}

const Peio::Gfx::RenderTargetHeap& Peio::Gfx::Graphics::GetRenderTargets() const noexcept
{
	return renderTargets;
}
