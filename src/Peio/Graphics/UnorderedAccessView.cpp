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

	void UnorderedAccessView::InitBuffer(const std::vector<UINT64>& sizes, const std::vector<UINT>& numElements, const std::vector<D3D12_RESOURCE_STATES>& states)
	{
		numResources = sizes.size();

		std::vector<D3D12_RESOURCE_DESC> resourceDescs(numResources);
		std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC> uavDescs(numResources);

		for (UINT i = 0; i < numResources; i++) {
			resourceDescs[i] = CD3DX12_RESOURCE_DESC::Buffer(sizes[i]);
			resourceDescs[i].Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			uavDescs[i].ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uavDescs[i].Format = DXGI_FORMAT_UNKNOWN;
			uavDescs[i].Buffer.FirstElement = 0;
			uavDescs[i].Buffer.NumElements = numElements[i];
			uavDescs[i].Buffer.StructureByteStride = (UINT)(sizes[i] / numElements[i]);
			uavDescs[i].Buffer.CounterOffsetInBytes = 0;
			uavDescs[i].Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		}
		Init(resourceDescs, states, uavDescs);
	}

	void UnorderedAccessView::InitTexture2D(const std::vector<Uint2>& sizes, const std::vector<DXGI_FORMAT>& formats, const std::vector<D3D12_RESOURCE_STATES>& states)
	{
		numResources = sizes.size();

		std::vector<D3D12_RESOURCE_DESC> resourceDescs(numResources);
		std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC> uavDescs(numResources);

		for (UINT i = 0; i < numResources; i++) {
			resourceDescs[i].Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			resourceDescs[i].Format = formats[i];
			resourceDescs[i].Width = sizes[i].x();
			resourceDescs[i].Height = sizes[i].y();
			resourceDescs[i].DepthOrArraySize = 1;
			resourceDescs[i].Alignment = 0;
			resourceDescs[i].Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resourceDescs[i].MipLevels = 1;
			resourceDescs[i].SampleDesc = { 1, 0 };
			resourceDescs[i].Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			uavDescs[i].ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			uavDescs[i].Format = formats[i];
			uavDescs[i].Texture2D = { 0, 0 };
		}
		Init(resourceDescs, states, uavDescs);
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