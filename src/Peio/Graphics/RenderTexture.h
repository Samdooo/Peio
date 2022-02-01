#pragma once

#include "Heap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT RenderTexture {

		void Init(DXGI_FORMAT format, Long2 size, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
		void SetRenderTarget(ID3D12GraphicsCommandList* cmdList) const;

		_NODISCARD DXGI_FORMAT GetFormat() const noexcept;
		_NODISCARD Long2 GetSize() const noexcept;
		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandle() const noexcept;
		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const noexcept;

		Heap heap = {};

	protected:

		DXGI_FORMAT format = {};
		Long2 size = {};
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = {};
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};

	};

}