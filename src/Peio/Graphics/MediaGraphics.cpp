#define PEIO_GFX_EXPORTING
#include "MediaGraphics.h"

namespace Peio::Gfx {

	void MediaGraphics::Init(Long2 size, UINT numBuffers)
	{
		MediaGraphics::~MediaGraphics();

		cmdQueue.Init();
		cmdList.Init(cmdQueue.GetQueue(), numBuffers, 2);

		bufferFootprint.Footprint.Width = (UINT)size.x();
		bufferFootprint.Footprint.Height = size.y();
		bufferFootprint.Footprint.Depth = 1;
		bufferFootprint.Footprint.RowPitch = (UINT)size.x() * 4;
		bufferFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		renderTargets.Init(numBuffers, size);
		renderTargets.Put();

		readbackResources.resize(numBuffers);
		copySrc.resize(numBuffers);
		copyDst.resize(numBuffers);
		buffers.resize(numBuffers);
		for (UINT i = 0; i < numBuffers; i++) {
			readbackResources[i].Init(D3D12_HEAP_TYPE_READBACK, CD3DX12_RESOURCE_DESC::Buffer(size.x() * size.y() * 4), D3D12_RESOURCE_STATE_COPY_DEST);
			readbackResources[i].Map((void**)&buffers[i], 0, true);
			copySrc[i] = CD3DX12_TEXTURE_COPY_LOCATION(renderTargets.GetRenderTarget(i)->GetResource(), 0);
			copyDst[i] = CD3DX12_TEXTURE_COPY_LOCATION(readbackResources[i].GetResource(), bufferFootprint);
		}
		mapRange = { 0, (UINT64)size.x() * (UINT64)size.y() * 4ULL };
	}

	void MediaGraphics::Clear(const Float4& color)
	{
		UINT frameIndex = renderTargets.GetFrameIndex();

		cmdList.Close();

		cmdQueue.Execute(cmdList.GetCommandLists(), 1);
		cmdList.GetAllocator(0, frameIndex).Signal(cmdQueue.GetQueue());

		//cmdList.GetAllocator(1, frameIndex).Wait();
		cmdList.Reset(1, frameIndex);

		renderTargets.GetRenderTarget(frameIndex)->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.GetCommandList());
		renderTargets.SetCurrent(cmdList.GetCommandList());

		renderTargets.ClearCurrent(cmdList.GetCommandList(), color);
	}

	void MediaGraphics::Render()
	{
		UINT frameIndex = renderTargets.GetFrameIndex();

		renderTargets.GetRenderTarget(frameIndex)->Transition(D3D12_RESOURCE_STATE_COPY_SOURCE, cmdList.GetCommandList());

		cmdList.GetCommandList()->CopyTextureRegion(&copyDst[frameIndex], 0, 0, 0, &copySrc[frameIndex], nullptr);

		renderTargets.GetRenderTarget(frameIndex)->Transition(D3D12_RESOURCE_STATE_PRESENT, cmdList.GetCommandList());

		cmdList.Close();
		cmdList.GetAllocator(0, frameIndex).Wait();

		cmdQueue.Execute(cmdList.GetCommandLists(), 1);
		cmdList.GetAllocator(1, frameIndex).Signal(cmdQueue.GetQueue());

		cmdList.GetAllocator(1, frameIndex).Wait();
		//cmdList.Reset(1, frameIndex);

		renderTargets.Next();
		cmdList.Reset(0, frameIndex);
	}

	MediaGraphics::~MediaGraphics()
	{
		for (UINT i = 0; i < readbackResources.size(); i++) {
			if (readbackResources[i].HasBuffer())
				readbackResources[i].GetResource()->Unmap(0, nullptr);
		}
	}

	byte* MediaGraphics::GetBuffer() const
	{
		return buffers[(renderTargets.GetFrameIndex() + renderTargets.GetNumDescriptors() - 1) % renderTargets.GetNumDescriptors()];
		//return buffers[renderTargets.GetFrameIndex()];
	}

}