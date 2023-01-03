#pragma once

#include "Global.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Resource {

		/// <summary>
		/// Releases any existing resource before initializing a new one.
		/// </summary>
		void Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE); // Always copy footprints (?)

		/// <summary>
		/// Transitions the resource's state to a new one.
		/// </summary>
		void Transition(D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList);

		/// <summary>
		/// Sets the resource's current state without affecting the resource.
		/// </summary>
		void SetState(D3D12_RESOURCE_STATES state);

		void Map(void** ptr, UINT subResource = 0, bool cpuRead = false);

		_NODISCARD ID3D12Resource* GetResource() const noexcept;
		_NODISCARD ID3D12Resource** GetResourcePtr() noexcept;
		_NODISCARD D3D12_RESOURCE_DESC GetResourceDesc() const noexcept;

		_NODISCARD bool HasBuffer() const noexcept;

		_NODISCARD D3D12_RESOURCE_STATES GetState() const noexcept;
		_NODISCARD D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const noexcept;

		virtual ~Resource();

	protected:

		ComPtr<ID3D12Resource> resource = nullptr;
		D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

		void CheckResource() const;

	};

}