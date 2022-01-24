#define PEIO_GFX_EXPORTING
#include "DescriptorHeap.h"

void Peio::Gfx::DescriptorHeap::CheckHeap() const
{
	if (!heap.Get()) {
		throw PEIO_EXCEPTION("No heap is currently allocated.");
	}
}

void Peio::Gfx::DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
	HRESULT ret;
	Release();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = type;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags = flags;
	heapDesc.NodeMask = 0;

	ret = device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), &heap);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create descriptor heap.", ret);
	}
	handleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
}

ID3D12DescriptorHeap* Peio::Gfx::DescriptorHeap::GetHeap() const noexcept
{
	return heap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE Peio::Gfx::DescriptorHeap::GetCPUHandle() const
{
	CheckHeap();
	return heap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_GPU_DESCRIPTOR_HANDLE Peio::Gfx::DescriptorHeap::GetGPUHandle() const
{
	CheckHeap();
	return heap->GetGPUDescriptorHandleForHeapStart();
}

UINT Peio::Gfx::DescriptorHeap::GetHandleIncrementSize() const noexcept
{
	return handleIncrementSize;
}

void Peio::Gfx::DescriptorHeap::Release()
{
	if (heap) {
		heap.ReleaseAndGetAddressOf();
		heap = nullptr;
	}
}

Peio::Gfx::DescriptorHeap::~DescriptorHeap()
{
	Release();
}
