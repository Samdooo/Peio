#define PEIO_GFX_EXPORTING
#include "UnorderedAccessView.h"

namespace Peio::Gfx {

	void UnorderedAccessView::Init(const std::vector<D3D12_RESOURCE_DESC>& descs, const std::vector<D3D12_RESOURCE_STATES>& states, const std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC>& uavDescs)
	{
		Release();

		numResources = (UINT)descs.size();
		descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)numResources, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();

		resources = new Resource[numResources];

		for (UINT i = 0; i < numResources; i++) {
			resources[i].Init(descs[i], states[i], true);

			device->CreateUnorderedAccessView(resources[i].GetBuffer(), nullptr, &uavDescs[i], handle);

			handle.Offset(descriptorHeap.GetHandleIncrementSize());
		}

		descriptorHeaps[0] = descriptorHeap.GetHeap();
	}

	DescriptorHeap& UnorderedAccessView::GetDescriptorHeap() noexcept
	{
		return descriptorHeap;
	}

	ID3D12DescriptorHeap** UnorderedAccessView::GetDescriptorHeaps() const noexcept
	{
		return descriptorHeaps;
	}

	Resource* UnorderedAccessView::GetResources() const noexcept
	{
		return resources;
	}

	UINT UnorderedAccessView::GetNumResources() const noexcept
	{
		return numResources;
	}

	void UnorderedAccessView::Release()
	{
		if (resources) {
			delete[] resources;
			resources = nullptr;
		}
		descriptorHeap.Release();
	}

	UnorderedAccessView::~UnorderedAccessView()
	{
		Release();
	}

}