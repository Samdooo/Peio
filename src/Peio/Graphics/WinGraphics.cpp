#define PEIO_GFX_EXPORTING
#include "WinGraphics.h"

void Peio::Gfx::WinGraphics::Init(HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen)
{
	WinGraphics::~WinGraphics();

	this->size = size;
	cmdQueue.Init();
	cmdList.Init(cmdQueue.GetQueue(), numBuffers, 2);

	swapChain.Init(cmdQueue.GetQueue(), hwnd, size, numBuffers, fullscreen);
	renderTargets.Init(numBuffers);
	for (UINT i = 0; i < numBuffers; i++) {
		swapChain.GetBuffer(i, renderTargets.GetRenderTarget(i)->GetResourcePtr());
	}
	renderTargets.Put();
}

void Peio::Gfx::WinGraphics::Clear(const Float4& color)
{
	UINT frameIndex = renderTargets.GetFrameIndex();

	cmdList.Close();

	cmdQueue.Execute(cmdList.GetCommandLists(), 1);
	cmdList.GetAllocator(0, frameIndex).Signal(cmdQueue.GetQueue());

	cmdList.GetAllocator(1, frameIndex).Wait();
	cmdList.Reset(1, frameIndex);

	renderTargets.GetRenderTarget(frameIndex)->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
	renderTargets.SetCurrent(cmdList.GetCommandList());
	
	renderTargets.ClearCurrent(cmdList.GetCommandList(), color);
}

void Peio::Gfx::WinGraphics::Render()
{
	UINT frameIndex = renderTargets.GetFrameIndex();

	renderTargets.GetRenderTarget(frameIndex)->Transition(D3D12_RESOURCE_STATE_PRESENT, cmdList.GetCommandList());

	cmdList.Close();
	cmdList.GetAllocator(0, frameIndex).Wait();

	cmdQueue.Execute(cmdList.GetCommandLists(), 1);
	cmdList.GetAllocator(1, frameIndex).Signal(cmdQueue.GetQueue());

	frameIndex = swapChain.SwapBuffers();
	renderTargets.SetFrameIndex(frameIndex);

	cmdList.Reset(0, frameIndex);
}

Peio::Gfx::WinGraphics::~WinGraphics()
{
}
