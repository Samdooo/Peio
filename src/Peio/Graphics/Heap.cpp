#define PEIO_GFX_EXPORTING
#include "Heap.h"

void Peio::Gfx::Heap::CheckBuffer() const
{
	if (!buffer.Get()) {
		throw PEIO_EXCEPTION("No buffer is currently allocated.");
	}
}

void Peio::Gfx::Heap::Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints, D3D12_HEAP_FLAGS heapFlags)
{
	HRESULT ret;
	Release();

	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(heapType);

	if (copyFootprints) {
		UINT64 size;
		device->GetCopyableFootprints(&resourceDesc, 0, 1, 0, nullptr, nullptr, nullptr, &size);
		resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	}

	ret = device->CreateCommittedResource(&properties, heapFlags, &resourceDesc, resourceState, nullptr, __uuidof(ID3D12Resource), &buffer);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create heap.", ret);
	}
	this->resourceState = resourceState;
}

void Peio::Gfx::Heap::Init(D3D12_HEAP_TYPE heapType, UINT64 size, D3D12_RESOURCE_STATES resourceState, bool copyFootprints, D3D12_HEAP_FLAGS heapFlags)
{
	Init(heapType, CD3DX12_RESOURCE_DESC::Buffer(size), resourceState, copyFootprints, heapFlags);
}

void Peio::Gfx::Heap::Transition(D3D12_RESOURCE_STATES resourceState, ID3D12GraphicsCommandList* cmdList)
{
	CheckBuffer();
	if (resourceState == this->resourceState)
		return;
	if (!cmdList) {
		throw PEIO_EXCEPTION("cmdList is nullptr.");
	}

	CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.Get(), this->resourceState, resourceState);
	cmdList->ResourceBarrier(1, &resourceBarrier);
}

void Peio::Gfx::Heap::SetResourceState(D3D12_RESOURCE_STATES resourceState)
{
	this->resourceState = resourceState;
}

void Peio::Gfx::Heap::Map(void** ptr, UINT subResource, bool cpuRead)
{
	CD3DX12_RANGE readRange(0, 0);
	HRESULT ret = buffer->Map(subResource, cpuRead ? nullptr : &readRange, ptr);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to map heap.", ret);
	}
}

ID3D12Resource* Peio::Gfx::Heap::GetBuffer() noexcept
{
	return buffer.Get();
}

ID3D12Resource** Peio::Gfx::Heap::GetBufferPtr() noexcept
{
	return &buffer;
}

//ID3D12Resource** Peio::Gfx::Heap::operator&() noexcept
//{
//	return &buffer;
//}

bool Peio::Gfx::Heap::HasBuffer() noexcept
{
	return buffer;
}

D3D12_RESOURCE_STATES Peio::Gfx::Heap::GetResourceState() noexcept
{
	return resourceState;
}

D3D12_GPU_VIRTUAL_ADDRESS Peio::Gfx::Heap::GetGPUAddress()
{
	CheckBuffer();
	return buffer->GetGPUVirtualAddress();
}

void Peio::Gfx::Heap::Release()
{
	if (buffer) {
		buffer.ReleaseAndGetAddressOf();
		buffer = nullptr;
	}
}

Peio::Gfx::Heap::~Heap()
{
	Release();
}
