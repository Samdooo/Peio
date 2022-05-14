#define PEIO_GFX_EXPORTING
#include "UnorderedAccessView.h"

namespace Peio::Gfx {

	//void UnorderedAccessView::Init(const std::vector<D3D12_RESOURCE_DESC>& descs, const std::vector<D3D12_RESOURCE_STATES>& states, const std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC>& uavDescs)
	//{
	//	Release();
	//
	//	numResources = (UINT)descs.size();
	//	descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)numResources, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	//
	//	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
	//
	//	resources = new Resource[numResources];
	//
	//	for (UINT i = 0; i < numResources; i++) {
	//		resources[i].Init(descs[i], states[i], true);
	//
	//		device->CreateUnorderedAccessView(resources[i].GetBuffer(), nullptr, &uavDescs[i], handle);
	//
	//		handle.Offset(descriptorHeap.GetHandleIncrementSize());
	//	}
	//
	//	descriptorHeaps[0] = descriptorHeap.GetHeap();
	//}
	//
	//void UnorderedAccessView::InitBuffer(const std::vector<UINT64>& sizes, const std::vector<UINT>& numElements, const std::vector<D3D12_RESOURCE_STATES>& states)
	//{
	//	numResources = sizes.size();
	//
	//	std::vector<D3D12_RESOURCE_DESC> resourceDescs(numResources);
	//	std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC> uavDescs(numResources);
	//
	//	for (UINT i = 0; i < numResources; i++) {
	//		resourceDescs[i] = CD3DX12_RESOURCE_DESC::Buffer(sizes[i]);
	//		resourceDescs[i].Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	//
	//		uavDescs[i].ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	//		uavDescs[i].Format = DXGI_FORMAT_UNKNOWN;
	//		uavDescs[i].Buffer.FirstElement = 0;
	//		uavDescs[i].Buffer.NumElements = numElements[i];
	//		uavDescs[i].Buffer.StructureByteStride = (UINT)(sizes[i] / numElements[i]);
	//		uavDescs[i].Buffer.CounterOffsetInBytes = 0;
	//		uavDescs[i].Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	//	}
	//	Init(resourceDescs, states, uavDescs);
	//}
	//
	//void UnorderedAccessView::InitTexture2D(const std::vector<Uint2>& sizes, const std::vector<DXGI_FORMAT>& formats, const std::vector<D3D12_RESOURCE_STATES>& states)
	//{
	//	numResources = sizes.size();
	//
	//	std::vector<D3D12_RESOURCE_DESC> resourceDescs(numResources);
	//	std::vector<D3D12_UNORDERED_ACCESS_VIEW_DESC> uavDescs(numResources);
	//
	//	for (UINT i = 0; i < numResources; i++) {
	//		resourceDescs[i].Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//		resourceDescs[i].Format = formats[i];
	//		resourceDescs[i].Width = sizes[i].x();
	//		resourceDescs[i].Height = sizes[i].y();
	//		resourceDescs[i].DepthOrArraySize = 1;
	//		resourceDescs[i].Alignment = 0;
	//		resourceDescs[i].Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//		resourceDescs[i].MipLevels = 1;
	//		resourceDescs[i].SampleDesc = { 1, 0 };
	//		resourceDescs[i].Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	//
	//		uavDescs[i].ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	//		uavDescs[i].Format = formats[i];
	//		uavDescs[i].Texture2D = { 0, 0 };
	//	}
	//	Init(resourceDescs, states, uavDescs);
	//}

	void UnorderedAccessView::Init(UINT numResources)
	{
		Release();
		resources.resize(numResources);
		descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numResources, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		descriptorHeaps[0] = descriptorHeap.GetHeap();
	}

	void UnorderedAccessView::CreateUAV(UINT index, D3D12_RESOURCE_DESC resourceDesc, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_STATES resourceState)
	{
		resources[index]->Init(resourceDesc, resourceState, true);
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();
		if (index)
			handle.Offset(index, descriptorHeap.GetHandleIncrementSize());
		device->CreateUnorderedAccessView(resources[index]->GetBuffer(), nullptr, &uavDesc, handle);
	}

	void UnorderedAccessView::CreateBufferUAV(UINT index, UINT64 size, UINT64 numElements, D3D12_RESOURCE_STATES resourceState)
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

	void UnorderedAccessView::SetShaderVisibility(D3D12_SHADER_VISIBILITY visibility)
	{
		this->visibility = visibility;
	}

	DescriptorHeap& UnorderedAccessView::GetDescriptorHeap() noexcept
	{
		return descriptorHeap;
	}

	ID3D12DescriptorHeap** UnorderedAccessView::GetDescriptorHeaps() const noexcept
	{
		return descriptorHeaps;
	}

	Resource** UnorderedAccessView::GetResources() noexcept
	{
		return &resources[0];
	}

	UINT UnorderedAccessView::GetNumResources() const noexcept
	{
		return (UINT)resources.size();
	}

	D3D12_SHADER_VISIBILITY UnorderedAccessView::GetVisibility() const noexcept
	{
		return visibility;
	}

	void UnorderedAccessView::Release()
	{
		descriptorHeap.Release();
	}

	UnorderedAccessView::~UnorderedAccessView()
	{
		Release();
	}

}