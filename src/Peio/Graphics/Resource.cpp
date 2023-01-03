#define PEIO_GFX_EXPORTING
#include "Resource.h"

namespace Peio::Gfx {

	void Resource::Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		HRESULT ret;
		Resource::~Resource();

		if (heapType == D3D12_HEAP_TYPE_UPLOAD)
			state = D3D12_RESOURCE_STATE_GENERIC_READ;
		else if (heapType == D3D12_HEAP_TYPE_READBACK)
			state = D3D12_RESOURCE_STATE_COPY_DEST;

		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(heapType);

		if (heapType == D3D12_HEAP_TYPE_UPLOAD) {
			UINT64 bufferSize = 0;
			device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &bufferSize);
			//D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Alignment = 0;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.Width = bufferSize;
			desc.Height = 1;
		}

		ret = device->CreateCommittedResource(&heapProps, heapFlags, &desc, state, nullptr, __uuidof(ID3D12Resource), &resource);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create heap.", ret);
		}
		this->state = state;
	}

	void Resource::Transition(D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList)
	{
		CheckResource();
		if (targetState == state)
			return;
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), state, targetState);
		cmdList->ResourceBarrier(1, &resourceBarrier);
	}

	void Resource::SetState(D3D12_RESOURCE_STATES state)
	{
		this->state = state;
	}

	void Resource::Map(void** ptr, UINT subResource, bool cpuRead)
	{
		CD3DX12_RANGE readRange(0, 0);
		HRESULT ret = resource->Map(subResource, cpuRead ? nullptr : &readRange, ptr);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to map resource.", ret);
		}
	}

	ID3D12Resource* Resource::GetResource() const noexcept
	{
		return resource.Get();
	}

	ID3D12Resource** Resource::GetResourcePtr() noexcept
	{
		return &resource;
	}

	D3D12_RESOURCE_DESC Resource::GetResourceDesc() const noexcept
	{
		return resource->GetDesc();
	}

	bool Resource::HasBuffer() const noexcept
	{
		return (bool)resource;
	}

	D3D12_RESOURCE_STATES Resource::GetState() const noexcept
	{
		return state;
	}

	D3D12_GPU_VIRTUAL_ADDRESS Resource::GetGPUAddress() const noexcept
	{
		return resource->GetGPUVirtualAddress();
	}

	Resource::~Resource()
	{
	}

	void Resource::CheckResource() const
	{
		if (!resource.Get()) {
			throw PEIO_EXCEPTION("Resource is not allocated.");
		}
	}

}