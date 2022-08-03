#define PEIO_GFX_EXPORTING
#include "ResourceArray.h"

namespace Peio::Gfx {

	void ResourceArray::Init(D3D12_SHADER_VISIBILITY visibility)
	{
		Release();
		this->visibility = visibility;
	}

	void ResourceArray::AddResource(ShaderResource* resource)
	{
		resources.push_back(resource);
	}

	void ResourceArray::CreateResources()
	{
		descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)resources.size(), D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		descriptorHeaps[0] = descriptorHeap.GetHeap();

		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
		for (UINT i = 0; i < (UINT)resources.size(); i++) {
			resources[i]->Create(handle);
			handle.Offset(1, descriptorHeap.GetHandleIncrementSize());
		}
	}

	ID3D12DescriptorHeap** ResourceArray::GetDescriptorHeaps() const noexcept
	{
		return descriptorHeaps;
	}

	DescriptorHeap& ResourceArray::GetDescriptorHeap() noexcept
	{
		return descriptorHeap;
	}

	const DescriptorHeap& ResourceArray::GetDescriptorHeap() const noexcept
	{
		return descriptorHeap;
	}

	ShaderResource** ResourceArray::GetResources() noexcept
	{
		return &resources[0];
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
