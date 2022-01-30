#define PEIO_GFX_EXPORTING
#include "RenderTexture.h"

namespace Peio::Gfx {

	void RenderTexture::Init(DXGI_FORMAT format, Long2 size, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_RESOURCE_FLAGS resourceFlags)
	{
		this->format = format;
		this->size = size;
		this->srvHandle = srvHandle;
		this->rtvHandle = rtvHandle;

		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, (UINT64)size.x(), size.y(), 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		heap.Init(D3D12_HEAP_TYPE_DEFAULT, resourceDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, false, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES);

		device->CreateRenderTargetView(heap.GetBuffer(), nullptr, rtvHandle);
		device->CreateShaderResourceView(heap.GetBuffer(), nullptr, srvHandle);

		heap.SetResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	DXGI_FORMAT RenderTexture::GetFormat() const noexcept
	{
		return format;
	}

	Long2 RenderTexture::GetSize() const noexcept
	{
		return size;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetSrvHandle() const noexcept
	{
		return srvHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetRtvHandle() const noexcept
	{
		return rtvHandle;
	}



}