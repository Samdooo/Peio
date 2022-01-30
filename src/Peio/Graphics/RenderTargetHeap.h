#pragma once

#include "DescriptorHeap.h"
#include "Heap.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT RenderTargetHeap {

	protected:

		DescriptorHeap heap = {};
		std::vector<Heap> renderTargets = {};
		UINT numBuffers = 0;
		UINT frameIndex = 0;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};

	public:

		void Init(UINT numBuffers);
		void CreateRenderTargets();
		void SetFrameIndex(UINT frameIndex);

		void SetRenderTarget(ID3D12GraphicsCommandList* cmdList) const;
		void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList, const Float4& color) const;

		_NODISCARD Heap* GetRenderTargets() noexcept;
		_NODISCARD Heap& GetCurrentRenderTarget() noexcept;
		_NODISCARD UINT GetNumBuffers() const noexcept;
		_NODISCARD UINT GetFrameIndex() const noexcept;

		void Release();

		~RenderTargetHeap();

	};

}