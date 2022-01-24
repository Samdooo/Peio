#define PEIO_GFX_EXPORTING
#include "UnorderedAccessView.h"

#include <iostream>

void Peio::Gfx::UnorderedAccessView::Init(const std::vector<UINT64>& sizes, const std::vector<UINT64>& numElements, ID3D12GraphicsCommandList* cmdList)
{
	std::cout << "Initializing uav" << std::endl;

	descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)sizes.size(), D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();

	resources = new Resource[sizes.size()];
	//ptrs = new void* [sizes.size()];

	for (size_t i = 0; i < sizes.size(); i++) {
		//heaps[i].Init(D3D12_HEAP_TYPE_UPLOAD, sizes[i] * numElements[i], D3D12_RESOURCE_STATE_GENERIC_READ); /* 64kb aligned */
		//heaps[i].Init(D3D12_HEAP_TYPE_UPLOAD, ((sizes[i] * numElements[i] - 1) / (1024 * 64) + 1) * (1024 * 64), D3D12_RESOURCE_STATE_GENERIC_READ); /* 64kb aligned */
		//resources[i].Init(CD3DX12_RESOURCE_DESC::Buffer(sizes[i] * numElements[i]), D3D12_RESOURCE_STATE_GENERIC_READ);
		resources[i].Init(CD3DX12_RESOURCE_DESC::Buffer(sizes[i] * numElements[i]
			, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS), D3D12_RESOURCE_STATE_GENERIC_READ);

		

		//resources[i].Transition(D3D12_RESOURCE_STATE_UNORDERED_ACCESS, cmdList);

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.NumElements = (UINT)numElements[i];
		desc.Buffer.StructureByteStride = (UINT)sizes[i];
		desc.Buffer.CounterOffsetInBytes = 0;
		desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		device->CreateUnorderedAccessView(resources[i].GetBuffer(), nullptr, &desc, handle);

		//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		//cbvDesc.BufferLocation = heaps[i].GetGPUAddress();
		//cbvDesc.SizeInBytes = (sizes[i] + 255) & ~255; /* 256b aligned */
		//device->CreateConstantBufferView(&cbvDesc, handle);

		//heaps[i].Map(&ptrs[i]);

		handle.Offset(descriptorHeap.GetHandleIncrementSize());
	}
}

Peio::Gfx::DescriptorHeap& Peio::Gfx::UnorderedAccessView::GetDescriptorHeap() noexcept
{
	return descriptorHeap;
}

Peio::Gfx::Resource* Peio::Gfx::UnorderedAccessView::GetResources() const noexcept
{
	return resources;
}