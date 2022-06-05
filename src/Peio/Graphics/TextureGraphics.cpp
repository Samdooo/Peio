#define PEIO_GFX_EXPORTING
#include "TextureGraphics.h"

namespace Peio::Gfx {

	void TextureGraphics::Init(Long2 size)
	{
		//cmdQueue.Init();
		//cmdList.Init(cmdQueue.GetQueue(), 1, 2);
		//
		//renderTargets.Init(1);
		//
		//srvHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		//
		//D3D12_RESOURCE_DESC resourceDesc;
		//resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		//resourceDesc.Alignment = 0;
		//resourceDesc.DepthOrArraySize = 1;
		//resourceDesc.MipLevels = 1;
		//resourceDesc.SampleDesc = { 1, 0 };
		//resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		//resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		//resourceDesc.Width = size.x();
		//resourceDesc.Height = size.y();
		//resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//
		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1;
		//
		//renderTargets.GetRenderTargets()[0].Init(D3D12_HEAP_TYPE_DEFAULT, resourceDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, false, D3D12_HEAP_FLAG_NONE);
		////device->CreateShaderResourceView(renderTargets.GetRenderTargets()[0].GetBuffer(), &srvDesc, srvHeap.GetCPUHandle());
		//renderTargets.CreateRenderTargets();
		////renderTargets.SetFrameIndex(0);
		//frontBuffer = &renderTargets.GetRenderTargets()[0];
	
		this->size = size;
		cmdQueue.Init();
		cmdList.Init(cmdQueue.GetQueue(), 1, 2);

		srvHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		rtvHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		texture.Init(DXGI_FORMAT_R8G8B8A8_UNORM, size, srvHeap.GetCPUHandle(), rtvHeap.GetCPUHandle());
	}

	void TextureGraphics::Clear(const Float4& color)
	{
		cmdList.Close();

		cmdQueue.Execute(cmdList.GetCommandLists(), 1);
		cmdList.GetAllocator(0, 0).Signal(cmdQueue.GetQueue());

		//cmdList.GetAllocator(1, frameIndex).Wait();
		cmdList.Reset(1, 0);

		//renderTargets.GetCurrentRenderTarget().Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
		//renderTargets.SetRenderTarget(cmdList.GetCommandList());
		texture.heap.Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap.GetCPUHandle();
		cmdList.GetCommandList()->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		//
		//renderTargets.ClearRenderTarget(cmdList.GetCommandList(), color);
		cmdList.GetCommandList()->ClearRenderTargetView(rtvHandle, &color[0], 0, nullptr);
	}

	void TextureGraphics::Render()
	{
		//renderTargets.GetCurrentRenderTarget().Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, cmdList.GetCommandList());
		texture.heap.Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, cmdList.GetCommandList());

		cmdList.Close();
		cmdList.GetAllocator(0, 0).Wait();

		cmdQueue.Execute(cmdList.GetCommandLists(), 1);
		cmdList.GetAllocator(1, 0).Signal(cmdQueue.GetQueue());
		cmdList.GetAllocator(1, 0).Wait();

		cmdList.Reset(0, 0);
	}

	const RenderTexture& TextureGraphics::GetTexture() const noexcept
	{
		return texture;
	}

	const DescriptorHeap& TextureGraphics::GetSrvHeap() const noexcept
	{
		return srvHeap;
	}

	const DescriptorHeap& TextureGraphics::GetRtvHeap() const noexcept
	{
		return rtvHeap;
	}

}