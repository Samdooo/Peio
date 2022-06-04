#define PEIO_GFX_EXPORTING
#include "UnorderedAccessView.h"

namespace Peio::Gfx {

	void UnorderedAccessView::Init(D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints)
	{
		this->uavDesc = uavDesc;
		Resource::Init(resourceDesc, resourceState, copyFootprints);
	}

	void UnorderedAccessView::Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
	{
		device->CreateUnorderedAccessView(GetBuffer(), nullptr, &uavDesc, cpuHandle);
	}

	void BufferUAV::Init(UINT64 size, UINT numElements, D3D12_RESOURCE_STATES resourceState)
	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = (UINT)numElements;
		uavDesc.Buffer.StructureByteStride = (UINT)(size / numElements);
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		Resource::Init(resourceDesc, resourceState, true);
	}

}