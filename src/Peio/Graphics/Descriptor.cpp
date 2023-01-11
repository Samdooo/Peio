#define PEIO_GFX_EXPORTING
#include "Descriptor.h"

namespace Peio::Gfx {

	void RenderTargetDescriptor::Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		device->CreateRenderTargetView(resource->GetResource(), nullptr, cpuHandle);
	}
	
	void RenderTarget::Init(Long2 size, DXGI_FORMAT format)
	{
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		desc.Width = size.x();
		desc.Height = size.y();
		desc.Format = format;
		
		Resource::Init(D3D12_HEAP_TYPE_DEFAULT, desc, D3D12_RESOURCE_STATE_PRESENT, D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_ALLOW_DISPLAY);
	}

	void RenderTarget::Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		RenderTargetDescriptor::Put(this, cpuHandle);
	}

	void SingleDescriptor::Init(const Resource* resource)
	{
		cbvDesc.BufferLocation = resource->GetGPUAddress();
		cbvDesc.SizeInBytes = (UINT)resource->GetResourceDesc().Width;
	}

	void SingleDescriptor::Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		device->CreateConstantBufferView(&cbvDesc, cpuHandle);
	}

	void SingleResource::Init(UINT64 size, bool allocateVector)
	{
		BufferedResource::Init(size, allocateVector);
	}

	void ArrayDescriptor::Init(D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc)
	{
		this->srvDesc = srvDesc;
	}

	void ArrayDescriptor::Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		device->CreateShaderResourceView(resource->GetResource(), &srvDesc, cpuHandle);
	}

	UINT ArrayDescriptor::GetElementSize() const noexcept
	{
		return elementSize;
	}

	UINT ArrayDescriptor::GetNumElements() const noexcept
	{
		return numElements;
	}

	void ArrayBufferResource::Init(UINT elementSize, UINT numElements, bool allocateVector, D3D12_RESOURCE_STATES state)
	{
		BufferedResource::Init((UINT64)elementSize * (UINT64)numElements, allocateVector, D3D12_HEAP_TYPE_DEFAULT, state);
		this->elementSize = elementSize;
		this->numElements = numElements;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = numElements;
		srvDesc.Buffer.StructureByteStride = elementSize;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		ArrayDescriptor::Init(srvDesc);
	}

	void ArrayBufferResource::Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		ArrayDescriptor::Put(this, cpuHandle);
	}

	void RWArrayDescriptor::Init(D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc)
	{
		this->uavDesc = uavDesc;
	}

	void RWArrayDescriptor::Put(const Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		device->CreateUnorderedAccessView(resource->GetResource(), nullptr, &uavDesc, cpuHandle);
	}

	void RWArrayBufferResource::Init(UINT elementSize, UINT numElements, bool allocateVector, D3D12_RESOURCE_STATES state)
	{
		BufferedResource::Init((UINT64)elementSize * (UINT64)numElements, allocateVector, D3D12_HEAP_TYPE_DEFAULT, state);

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = numElements;
		uavDesc.Buffer.StructureByteStride = elementSize;
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		RWArrayDescriptor::Init(uavDesc);
	}

	void RWArrayBufferResource::Put(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
	{
		RWArrayDescriptor::Put(this, cpuHandle);
	}

	Descriptor::~Descriptor()
	{
	}

}