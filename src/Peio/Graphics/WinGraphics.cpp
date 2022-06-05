#define PEIO_GFX_EXPORTING
#include "WinGraphics.h"

void Peio::Gfx::WinGraphics::Init(HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen)
{
	Release();

	this->size = size;
	cmdQueue.Init();
	cmdList.Init(cmdQueue.GetQueue(), numBuffers, 2);

	swapChain.Init(cmdQueue.GetQueue(), hwnd, size, numBuffers, fullscreen);

	Resize(hwnd, size, numBuffers, fullscreen);
}

void Peio::Gfx::WinGraphics::Resize(HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen)
{
	this->size = size;
	swapChain.Resize(cmdQueue.GetQueue(), hwnd, size, numBuffers, fullscreen);

	renderTargets.Init(numBuffers);
	for (UINT i = 0; i < numBuffers; i++) {
		swapChain.GetBuffer(i, renderTargets.GetRenderTargets()[i].GetBufferPtr());
	}

	renderTargets.CreateRenderTargets();
}

void Peio::Gfx::WinGraphics::Clear(const Float4& color)
{
	UINT frameIndex = renderTargets.GetFrameIndex();

	cmdList.Close();

	cmdQueue.Execute(cmdList.GetCommandLists(), 1);
	cmdList.GetAllocator(0, frameIndex).Signal(cmdQueue.GetQueue());

	cmdList.GetAllocator(1, frameIndex).Wait();
	cmdList.Reset(1, frameIndex);

	renderTargets.GetCurrentRenderTarget().Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
	renderTargets.SetRenderTarget(cmdList.GetCommandList());
	
	renderTargets.ClearRenderTarget(cmdList.GetCommandList(), color);
}

void Peio::Gfx::WinGraphics::Render()
{
	UINT frameIndex = renderTargets.GetFrameIndex();

	renderTargets.GetCurrentRenderTarget().Transition(D3D12_RESOURCE_STATE_PRESENT, cmdList.GetCommandList());

	cmdList.Close();
	cmdList.GetAllocator(0, frameIndex).Wait();

	cmdQueue.Execute(cmdList.GetCommandLists(), 1);
	cmdList.GetAllocator(1, frameIndex).Signal(cmdQueue.GetQueue());

	frameIndex = swapChain.SwapBuffers();
	renderTargets.SetFrameIndex(frameIndex);

	cmdList.Reset(0, frameIndex);
}

void Peio::Gfx::WinGraphics::Release()
{
	cmdQueue.Release();
	cmdList.Release();
	swapChain.Release();
	renderTargets.Release();
}

Peio::Gfx::WinGraphics::~WinGraphics()
{
	Release();
}
