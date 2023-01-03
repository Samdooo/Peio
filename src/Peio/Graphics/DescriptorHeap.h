#pragma once

#include "Descriptor.h"
#include <vector>

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT DescriptorHeap {

		void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors = 1, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		void Put(Descriptor* descriptor, UINT index);

		_NODISCARD UINT GetNumDescriptors() const;
		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const;
		_NODISCARD D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const;

		~DescriptorHeap();

	protected:

		ComPtr<ID3D12DescriptorHeap> heap = nullptr;
		UINT handleIncrementSize = 0;

	};

	struct PEIO_GFX_EXPORT RenderTargetHeap : public DescriptorHeap {

		void Init(UINT numRenderTargets);
		void Init(UINT numRenderTargets, Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		
		void Put();

		void SetFrameIndex(UINT frameIndex);
		void Next();
		
		void SetCurrent(ID3D12GraphicsCommandList* cmdList);
		void ClearCurrent(ID3D12GraphicsCommandList* cmdList, Float4 color);

		_NODISCARD RenderTarget* GetRenderTarget(UINT index) noexcept;
		_NODISCARD UINT GetFrameIndex() const noexcept;

	protected:

		using DescriptorHeap::Put;

		std::vector<RenderTarget> renderTargets = {};

		UINT frameIndex = 0;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};

	};

}
