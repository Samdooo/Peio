#define PEIO_GFX_EXPORTING
#include "MultiPassGraphics.h"

namespace Peio::Gfx {

	void MultiPassGraphics::Init(DXGI_FORMAT format, Long2 size, UINT numBuffers, UINT numPasses)
	{
		this->numBuffers = numBuffers;
		this->numPasses = numPasses;

		srvHeaps.resize(numPasses);
		rtvHeaps.resize(numPasses);
		renderTargets.resize(numPasses);
		gpuHandles.resize(numPasses);

		for (UINT i = 0; i < numPasses; i++) {
			srvHeaps[i].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numBuffers, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
			rtvHeaps[i].Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numBuffers);

			renderTargets[i].resize(numBuffers);
			gpuHandles[i].resize(numBuffers);

			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(srvHeaps[i].GetCPUHandle());
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeaps[i].GetCPUHandle());

			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(srvHeaps[i].GetGPUHandle());

			for (UINT j = 0; j < numBuffers; j++) {
				renderTargets[i][j].Init(format, size, srvHandle, rtvHandle);
				gpuHandles[i][j] = gpuHandle;

				srvHandle.Offset(1, srvHeaps[i].GetHandleIncrementSize());
				rtvHandle.Offset(1, rtvHeaps[i].GetHandleIncrementSize());
				gpuHandle.Offset(1, srvHeaps[i].GetHandleIncrementSize());
			}
		}
	}

	void MultiPassGraphics::Reset()
	{
		passIndex = 0;
	}

	void MultiPassGraphics::BeginPass(ID3D12GraphicsCommandList* cmdList, UINT frameIndex, Float4 clearColor)
	{
		this->frameIndex = frameIndex;
		renderTargets[passIndex][frameIndex].heap.Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);
		renderTargets[passIndex][frameIndex].SetRenderTarget(cmdList);
		cmdList->ClearRenderTargetView(renderTargets[passIndex][frameIndex].GetRtvHandle(), &clearColor[0], 0, nullptr);
	}

	void MultiPassGraphics::EndPass(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES targetState)
	{
		renderTargets[passIndex][frameIndex].heap.Transition(targetState, cmdList);
		passIndex++;
		if (passIndex == numPasses)
			passIndex = 0;
	}

	const RenderTexture& MultiPassGraphics::GetPreviousTexture() const
	{
		return renderTargets[(passIndex == 0 ? numPasses : passIndex) - 1][frameIndex];
	}

	D3D12_GPU_DESCRIPTOR_HANDLE MultiPassGraphics::GetPreviousGpuHandle() const
	{
		return gpuHandles[(passIndex == 0 ? numPasses : passIndex) - 1][frameIndex];
	}

	const DescriptorHeap& MultiPassGraphics::GetPreviousSrv() const
	{
		return srvHeaps[(passIndex == 0 ? numPasses : passIndex) - 1];
	}

	const DescriptorHeap& MultiPassGraphics::GetPreviousRtv() const
	{
		return rtvHeaps[(passIndex == 0 ? numPasses : passIndex) - 1];
	}

	

	UINT MultiPassGraphics::GetPassIndex() const noexcept
	{
		return passIndex;
	}

}