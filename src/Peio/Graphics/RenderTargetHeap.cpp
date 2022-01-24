#define PEIO_GFX_EXPORTING
#include "RenderTargetHeap.h"

void Peio::Gfx::RenderTargetHeap::Init(UINT numBuffers)
{
	Release();

	heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numBuffers);
	renderTargets.resize(numBuffers);
	this->numBuffers = numBuffers;
}

void Peio::Gfx::RenderTargetHeap::CreateRenderTargets()
{
	if (!renderTargets.size()) {
		throw PEIO_EXCEPTION("RenderTargetHeap must be initialized before being created.");
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)heap.GetCPUHandle();

	for (UINT i = 0; i < numBuffers; i++) {
		device->CreateRenderTargetView(renderTargets[i].GetBuffer(), nullptr, handle);
		handle.Offset(1, heap.GetHandleIncrementSize());
		renderTargets[i].SetResourceState(D3D12_RESOURCE_STATE_PRESENT);
	}

	SetFrameIndex(0);
}

void Peio::Gfx::RenderTargetHeap::SetFrameIndex(UINT frameIndex)
{
	this->frameIndex = frameIndex;
	rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap.GetCPUHandle(), frameIndex, heap.GetHandleIncrementSize());
}

void Peio::Gfx::RenderTargetHeap::SetRenderTarget(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
}

void Peio::Gfx::RenderTargetHeap::ClearRenderTarget(ID3D12GraphicsCommandList* cmdList, const Float4& color)
{
	cmdList->ClearRenderTargetView(rtvHandle, &color[0], 0, nullptr);
}

Peio::Gfx::Heap* Peio::Gfx::RenderTargetHeap::GetRenderTargets() noexcept
{
	return &renderTargets[0];
}

Peio::Gfx::Heap& Peio::Gfx::RenderTargetHeap::GetCurrentRenderTarget() noexcept
{
	return renderTargets[frameIndex];
}

UINT Peio::Gfx::RenderTargetHeap::GetNumBuffers() const noexcept
{
	return numBuffers;
}

UINT Peio::Gfx::RenderTargetHeap::GetFrameIndex() const noexcept
{
	return frameIndex;
}

void Peio::Gfx::RenderTargetHeap::Release()
{
	renderTargets.clear();
	heap.Release();
}

Peio::Gfx::RenderTargetHeap::~RenderTargetHeap()
{
	Release();
}
