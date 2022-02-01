#pragma once

#include "RenderTexture.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT MultiPassGraphics {

		void Init(DXGI_FORMAT format, Long2 size, UINT numBuffers, UINT numPasses);

		void Reset();
		void BeginPass(ID3D12GraphicsCommandList* cmdList, UINT frameIndex, Float4 clearColor);
		void EndPass(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES targetState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		_NODISCARD const RenderTexture& GetPreviousTexture() const;
		_NODISCARD D3D12_GPU_DESCRIPTOR_HANDLE GetPreviousGpuHandle() const;
		_NODISCARD const DescriptorHeap& GetPreviousSrv() const;
		_NODISCARD const DescriptorHeap& GetPreviousRtv() const;
		_NODISCARD UINT GetPassIndex() const noexcept;

	protected:

		std::vector<std::vector<RenderTexture>> renderTargets = {};
		std::vector<std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>> gpuHandles = {};
		std::vector<DescriptorHeap> srvHeaps = {};
		std::vector<DescriptorHeap> rtvHeaps = {};

		UINT numBuffers = 0;
		UINT numPasses = 0;
		UINT frameIndex = 0;
		UINT passIndex = 0;

	};

}