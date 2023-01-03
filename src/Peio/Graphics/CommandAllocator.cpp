#define PEIO_GFX_EXPORTING
#include "CommandAllocator.h"

void Peio::Gfx::CommandAllocator::Init()
{
	HRESULT ret;
	CommandAllocator::~CommandAllocator();

	ret = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), &cmdAllocator);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create command allocator.", ret);
	}

	ret = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), &fence);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create fence.", ret);
	}
	
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!fenceEvent) {
		throw PEIO_EXCEPTION("Failed to create fence event.");
	}
}

void Peio::Gfx::CommandAllocator::Signal(ID3D12CommandQueue* cmdQueue)
{
	fenceTarget++;
	HRESULT ret = cmdQueue->Signal(fence.Get(), fenceTarget);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to signal fence.", ret);
	}
}

void Peio::Gfx::CommandAllocator::Wait() const
{
	if (fence->GetCompletedValue() < fenceTarget) {
		HRESULT ret = fence->SetEventOnCompletion(fenceTarget, fenceEvent);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to set fence event.", ret);
		}
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void Peio::Gfx::CommandAllocator::Reset() const
{
	HRESULT ret = cmdAllocator->Reset();
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to reset command allocator.", ret);
	}
}

ID3D12CommandAllocator* Peio::Gfx::CommandAllocator::GetAllocator() const noexcept
{
	return cmdAllocator.Get();
}

ID3D12Fence* Peio::Gfx::CommandAllocator::GetFence() const noexcept
{
	return fence.Get();
}

Peio::Gfx::CommandAllocator::~CommandAllocator()
{
	if (fenceEvent) {
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}
	fenceTarget = 0;
}
