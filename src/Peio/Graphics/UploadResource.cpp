#define PEIO_GFX_EXPORTING
#include "UploadResource.h"

namespace Peio::Gfx {

	void UploadResource::Init(D3D12_RESOURCE_DESC desc)
	{
		uploadResource.Init(D3D12_HEAP_TYPE_UPLOAD, desc);
	}

	void UploadResource::Upload(Resource* resource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
	{
		D3D12_RESOURCE_STATES oldState = resource->GetState();
		resource->Transition(D3D12_RESOURCE_STATE_COPY_DEST, cmdList);

		UpdateSubresources(cmdList, resource->GetResource(), uploadResource.GetResource(), 0, 0, 1, &data);

		resource->Transition(oldState, cmdList);
	}

	void BufferResource::Init(Resource* resource, UINT64 size, bool allocateVector, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		resource->Init(heapType, CD3DX12_RESOURCE_DESC::Buffer(size), state, heapFlags);
		if (allocateVector)
			buffer.GetVector().resize(size);
	}

	void BufferResource::Upload(Resource* resource, UploadResource* uploadResource, ID3D12GraphicsCommandList* cmdList)
	{
		data.pData = buffer.GetData();
		data.RowPitch = buffer.GetSize();
		data.SlicePitch = buffer.GetSize();
		uploadResource->Upload(resource, data, cmdList);
	}

	void UploadedResource::Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		Resource::Init(heapType, desc, state, heapFlags);
		UploadResource::Init(desc);
	}

	void UploadedResource::Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
	{
		UploadResource::Upload(this, data, cmdList);
	}

	void BufferedResource::Init(UINT64 size, bool allocateVector, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		BufferResource::Init(this, size, allocateVector, heapType, state, heapFlags);
		UploadResource::Init(Resource::GetResourceDesc());
	}

	void BufferedResource::Upload(ID3D12GraphicsCommandList* cmdList)
	{
		UploadResource::Upload(this, data, cmdList);
	}

}