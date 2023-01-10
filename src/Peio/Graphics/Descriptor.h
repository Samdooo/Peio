#pragma once

#include "UploadResource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Descriptor {

		virtual void Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const = 0;

		virtual ~Descriptor();

	};

	struct PEIO_GFX_EXPORT RenderTargetDescriptor : public Descriptor {

		void Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const;

	};

	struct PEIO_GFX_EXPORT RenderTarget : public Resource, public RenderTargetDescriptor {

		void Init(Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const override;

	};

	struct PEIO_GFX_EXPORT SingleDescriptor : public Descriptor {

		void Init(const Resource* resource);

		void Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const override;

	protected:

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	};

	struct PEIO_GFX_EXPORT SingleResource : public BufferedResource, public SingleDescriptor {

		void Init(UINT64 size, bool allocateVector = true);

	};

	struct PEIO_GFX_EXPORT ArrayDescriptor : public Descriptor {

		void Init(D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc);

		void Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const;

		_NODISCARD UINT GetElementSize() const noexcept;
		_NODISCARD UINT GetNumElements() const noexcept;

	protected:

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		UINT elementSize = 0, numElements = 0;

	};

	struct PEIO_GFX_EXPORT ArrayBufferResource : public BufferedResource, public ArrayDescriptor {

		void Init(UINT elementSize, UINT numElements, bool allocateVector = true);

		void Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const override;

	};

	// TextureResource

	
	struct PEIO_GFX_EXPORT RWArrayDescriptor : public Descriptor {

		void Init(D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc);

		void Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const;

	protected:

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	};

	struct PEIO_GFX_EXPORT RWArrayBufferResource : public BufferedResource, public RWArrayDescriptor {

		void Init(UINT elementSize, UINT numElements, bool allocateVector = true);

		void Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const override;

	};

}