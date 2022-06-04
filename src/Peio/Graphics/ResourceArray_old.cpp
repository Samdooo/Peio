#define PEIO_GFX_EXPORTING
#include "ResourceArray.h"

namespace Peio::Gfx {

	void ResourceArray::Init(UINT numResources)
	{
		Release();
		resources.resize(numResources);
		descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numResources, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		descriptorHeaps[0] = descriptorHeap.GetHeap();
	}

	void ResourceArray::SetShaderVisibility(D3D12_SHADER_VISIBILITY visibility)
	{
		this->visibility = visibility;
	}

	void ResourceArray::CreateSRV(UINT index, D3D12_RESOURCE_DESC resourceDesc, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, D3D12_RESOURCE_STATES resourceState)
	{
		resources[index]->Init(resourceDesc, resourceState, true);
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
		if (index)
			handle.Offset(index, descriptorHeap.GetHandleIncrementSize());
		device->CreateShaderResourceView(resources[index]->GetBuffer(), &srvDesc, handle);
	}

	void ResourceArray::CreateBufferSRV(UINT index, UINT64 size, UINT64 numElements, D3D12_RESOURCE_STATES resourceState)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Buffer.NumElements = (UINT)numElements;
		srvDesc.Buffer.StructureByteStride = (UINT)(size / numElements);

		CreateSRV(index, CD3DX12_RESOURCE_DESC::Buffer(size), srvDesc, resourceState);
	}

	void ResourceArray::CreateTexture2DSRV(UINT index, Uint2 size, DXGI_FORMAT format, D3D12_RESOURCE_STATES resourceState)
	{

	}

	void ResourceArray::CreateUAV(UINT index, D3D12_RESOURCE_DESC resourceDesc, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_STATES resourceState)
	{
		resources[index]->Init(resourceDesc, resourceState, true);
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
		if (index)
			handle.Offset(index, descriptorHeap.GetHandleIncrementSize());
		device->CreateUnorderedAccessView(resources[index]->GetBuffer(), nullptr, &uavDesc, handle);
	}

	void ResourceArray::CreateBufferUAV(UINT index, UINT64 size, UINT64 numElements, D3D12_RESOURCE_STATES resourceState)
	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = (UINT)numElements;
		uavDesc.Buffer.StructureByteStride = (UINT)(size / numElements);
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		CreateUAV(index, resourceDesc, uavDesc, resourceState);
	}

	ID3D12DescriptorHeap** ResourceArray::GetDescriptorHeaps() const noexcept
	{
		return descriptorHeaps;
	}

	DescriptorHeap& ResourceArray::GetDescriptorHeap() noexcept
	{
		return descriptorHeap;
	}

	Resource** ResourceArray::GetResources() noexcept
	{
		return &resources[0];
	}

	const ResourceArray::Type* ResourceArray::GetTypes() const noexcept
	{
		return &types[0];
	}

	UINT ResourceArray::GetNumResources() const noexcept
	{
		return (UINT)resources.size();
	}

	D3D12_SHADER_VISIBILITY ResourceArray::GetVisibility() const noexcept
	{
		return visibility;
	}

	void ResourceArray::Release()
	{
		descriptorHeap.Release();
	}

	ResourceArray::~ResourceArray()
	{
		Release();
	}

}