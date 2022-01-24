#define PEIO_GFX_EXPORTING
#include "ConstantBufferView.h"

void Peio::Gfx::ConstantBufferView::Init(const std::vector<UINT64>& sizes)
{
	descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (UINT)sizes.size(), D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = (CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap.GetCPUHandle();

	heaps = new Heap[sizes.size()];
	ptrs = new void*[sizes.size()];

	CD3DX12_RANGE readRange(0, 0);

	for (size_t i = 0; i < sizes.size(); i++) {
		heaps[i].Init(D3D12_HEAP_TYPE_UPLOAD, ((sizes[i] - 1) / (1024 * 64) + 1) * (1024 * 64), D3D12_RESOURCE_STATE_GENERIC_READ); /* 64kb aligned */

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = heaps[i].GetGPUAddress();
		cbvDesc.SizeInBytes = (sizes[i] + 255) & ~255; /* 256b aligned */
		device->CreateConstantBufferView(&cbvDesc, handle);

		heaps[i].Map(&ptrs[i]);

		handle.Offset(descriptorHeap.GetHandleIncrementSize());
	}
}

Peio::Gfx::DescriptorHeap& Peio::Gfx::ConstantBufferView::GetDescriptorHeap() noexcept
{
	return descriptorHeap;
}

Peio::Gfx::Heap* Peio::Gfx::ConstantBufferView::GetHeaps() const noexcept
{
	return heaps;
}

void** Peio::Gfx::ConstantBufferView::GetPointers() const noexcept
{
	return ptrs;
}
