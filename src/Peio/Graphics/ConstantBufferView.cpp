#define PEIO_GFX_EXPORTING
#include "ConstantBufferView.h"

namespace Peio::Gfx {

	void ConstantBufferView::Init(UINT64 size, D3D12_RESOURCE_STATES resourceState)
	{
		size = (size + 255) & ~255;
		Resource::Init(CD3DX12_RESOURCE_DESC::Buffer(size), resourceState, false);
		cbvDesc.BufferLocation = GetGPUAddress();
		cbvDesc.SizeInBytes = (UINT)size;
	}

	void ConstantBufferView::Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
	{
		device->CreateConstantBufferView(&cbvDesc, cpuHandle);
	}

	void ConstantBufferView::SetAsRootParameter(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const
	{
		cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, this->GetGPUAddress());
	}

}