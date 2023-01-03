#pragma once

#include "Resource.h"
#include "..\Buffer.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT UploadResource { // Maybe have Buffer here (actually, probably not)

		void Init(D3D12_RESOURCE_DESC desc);

		void Upload(Resource* resource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

	protected:

		Resource uploadResource = {};

	};

	struct PEIO_GFX_EXPORT BufferResource {

		void Init(Resource* resource, UINT64 size, bool allocateVector = true, D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
		
		void Upload(Resource* resource, UploadResource* uploadResource, ID3D12GraphicsCommandList* cmdList);

		Buffer<byte> buffer = {};

	protected:

		D3D12_SUBRESOURCE_DATA data = {};

	};

	struct PEIO_GFX_EXPORT UploadedResource : public Resource, public UploadResource { // What the hell is a good name?

		void Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

	};

	struct PEIO_GFX_EXPORT BufferedResource : public Resource, public UploadResource, public BufferResource {

		void Init(UINT64 size, bool allocateVector = true, D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Upload(ID3D12GraphicsCommandList* cmdList);

	};

}