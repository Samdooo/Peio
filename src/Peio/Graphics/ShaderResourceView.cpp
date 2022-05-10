#define PEIO_GFX_EXPORTING
#include "ShaderResourceView.h"

void Peio::Gfx::ShaderResourceView::Init(const std::vector<D3D12_RESOURCE_DESC>& descs, const std::vector<D3D12_RESOURCE_STATES>& states, const std::vector<D3D12_SHADER_RESOURCE_VIEW_DESC>& srvDescs)
{
	Release();

	numResources = (UINT)descs.size();
	descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)numResources, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
	
	resources = new Resource[numResources];

	for (UINT i = 0; i < numResources; i++) {
		resources[i].Init(descs[i], states[i], true);

		device->CreateShaderResourceView(resources[i].GetBuffer(), &srvDescs[i], handle);

		handle.Offset(descriptorHeap.GetHandleIncrementSize());
	}

	descriptorHeaps[0] = descriptorHeap.GetHeap();
}

void Peio::Gfx::ShaderResourceView::InitBuffer(const std::vector<UINT64>& sizes, const std::vector<UINT>& numElements, const std::vector<D3D12_RESOURCE_STATES>& states)
{
	std::vector<D3D12_RESOURCE_DESC> descs(sizes.size());
	std::vector<D3D12_SHADER_RESOURCE_VIEW_DESC> srvDescs(sizes.size());
	for (size_t i = 0; i < sizes.size(); i++) {
		descs[i] = CD3DX12_RESOURCE_DESC::Buffer(sizes[i]);
		
		srvDescs[i].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDescs[i].Format = DXGI_FORMAT_UNKNOWN;
		srvDescs[i].ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDescs[i].Buffer.FirstElement = 0;
		srvDescs[i].Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDescs[i].Buffer.NumElements = numElements[i];
		srvDescs[i].Buffer.StructureByteStride = (UINT)(sizes[i] / numElements[i]);
	}

	Init(descs, states, srvDescs);
}

Peio::Gfx::DescriptorHeap& Peio::Gfx::ShaderResourceView::GetDescriptorHeap() noexcept
{
	return descriptorHeap;
}

ID3D12DescriptorHeap** Peio::Gfx::ShaderResourceView::GetDescriptorHeaps() const noexcept
{
	return descriptorHeaps;
}

Peio::Gfx::Resource* Peio::Gfx::ShaderResourceView::GetResources() const noexcept
{
	return resources;
}

UINT Peio::Gfx::ShaderResourceView::GetNumResources() const noexcept
{
	return numResources;
}

void Peio::Gfx::ShaderResourceView::Release()
{
	if (resources) {
		delete[] resources;
		resources = nullptr;
	}
	descriptorHeap.Release();
}

Peio::Gfx::ShaderResourceView::~ShaderResourceView()
{
	Release();
}
