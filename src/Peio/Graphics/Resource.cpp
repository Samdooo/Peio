#define PEIO_GFX_EXPORTING
#include "Resource.h"

void Peio::Gfx::Resource::Init(D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints)
{
	Release();

	Heap::Init(D3D12_HEAP_TYPE_DEFAULT, resourceDesc, resourceState);
	uploadHeap.Init(D3D12_HEAP_TYPE_UPLOAD, resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, copyFootprints);
}

void Peio::Gfx::Resource::InitBuffer(UINT64 size, D3D12_RESOURCE_STATES resourceState, bool copyFootprints)
{
	Init(CD3DX12_RESOURCE_DESC::Buffer(size), resourceState, copyFootprints);
}

void Peio::Gfx::Resource::Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
{
	D3D12_RESOURCE_STATES oldState = resourceState;
	Transition(D3D12_RESOURCE_STATE_COPY_DEST, cmdList);

	UpdateSubresources(cmdList, buffer.Get(), uploadHeap.GetBuffer(), 0, 0, 1, &data);

	Transition(oldState, cmdList);
}

void Peio::Gfx::Resource::Release()
{
	Heap::Release();
	uploadHeap.Release();
}

Peio::Gfx::Resource::~Resource()
{
	Release();
}
