#define PEIO_GFX_EXPORTING
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	void DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		HRESULT ret;
		DescriptorHeap::~DescriptorHeap();

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = flags;
		heapDesc.NodeMask = 0;

		ret = device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), &heap);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create descriptor heap.", ret);
		}
		handleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
	}

	void DescriptorHeap::Put(Descriptor* descriptor, UINT index)
	{
		descriptor->Put(((CD3DX12_CPU_DESCRIPTOR_HANDLE)heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, handleIncrementSize));
	}

	UINT DescriptorHeap::GetNumDescriptors() const
	{
		return heap->GetDesc().NumDescriptors;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandle() const
	{
		return heap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandle() const
	{
		return heap->GetGPUDescriptorHandleForHeapStart();
	}

	DescriptorHeap::~DescriptorHeap()
	{
	}


	void RenderTargetHeap::Init(UINT numRenderTargets)
	{
		DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numRenderTargets);
		renderTargets.resize(numRenderTargets);
	}

	void RenderTargetHeap::Init(UINT numRenderTargets, Long2 size, DXGI_FORMAT format)
	{
		Init(numRenderTargets);
		for (UINT i = 0; i < numRenderTargets; i++) {
			renderTargets[i].Init(size, format);
		}
	}

	void RenderTargetHeap::Put()
	{
		for (UINT i = 0; i < renderTargets.size(); i++) {
			DescriptorHeap::Put(&renderTargets[i], i);
		}
		SetFrameIndex(0);
	}

	void RenderTargetHeap::SetFrameIndex(UINT frameIndex)
	{
		this->frameIndex = frameIndex;
		rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap::GetCPUHandle(), frameIndex, DescriptorHeap::handleIncrementSize);
	}

	void RenderTargetHeap::Next()
	{
		SetFrameIndex((frameIndex + 1) % DescriptorHeap::GetNumDescriptors());
	}

	void RenderTargetHeap::SetCurrent(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	}

	void RenderTargetHeap::ClearCurrent(ID3D12GraphicsCommandList* cmdList, Float4 color)
	{
		cmdList->ClearRenderTargetView(rtvHandle, &color[0], 0, nullptr);
	}

	RenderTarget* RenderTargetHeap::GetRenderTarget(UINT index) noexcept
	{
		return &renderTargets[index];
	}

	UINT RenderTargetHeap::GetFrameIndex() const noexcept
	{
		return frameIndex;
	}

}
