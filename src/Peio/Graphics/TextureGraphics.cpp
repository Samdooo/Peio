#define PEIO_GFX_EXPORTING
#include "TextureGraphics.h"

namespace Peio::Gfx {

	void TextureGraphics::Init(Long2 size)
	{
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

		cmdList.Reset(1, 0);

		texture.heap.Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap.GetCPUHandle();
		cmdList.GetCommandList()->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		cmdList.GetCommandList()->ClearRenderTargetView(rtvHandle, &color[0], 0, nullptr);
	}

	void TextureGraphics::Render()
	{
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