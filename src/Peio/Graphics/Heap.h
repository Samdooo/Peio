#pragma once

#include "Global.h"

namespace Peio::Gfx {

	/// <summary>
	/// A helper struct for an ID3D12Resource. Used for both CPU and GPU buffers.
	/// </summary>
	class PEIO_GFX_EXPORT Heap {

	protected:

		Microsoft::WRL::ComPtr<ID3D12Resource> buffer = nullptr;
		D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_COMMON;

		void CheckBuffer() const;

	public:

		/// <summary>
		/// Releases any existing buffer before initializing a new buffer.
		/// </summary>
		/// <param name="heapType">The heap type.</param>
		/// <param name="resourceDesc">The resource descriptor.</param>
		/// <param name="resourceState">The initial resource state.</param>
		/// <param name="copyFootprints">Specifies if the resource layout should be changed to a copyable one. Typically used for upload heaps.</param>
		/// <param name="heapFlags">Heap flags.</param>
		void Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		/// <summary>
		/// Releases any existing buffer before initializing a new buffer.
		/// </summary>
		/// <param name="heapType">The heap type.</param>
		/// <param name="size">The size in bytes of the heap.</param>
		/// <param name="resourceState">The initial resource state.</param>
		/// <param name="copyFootprints">Specifies if the resource layout should be changed to a copyable one. Typically used for upload heaps.</param>
		/// <param name="heapFlags">Heap flags.</param>
		void Init(D3D12_HEAP_TYPE heapType, UINT64 size, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		/// <summary>
		/// Transitions to a new resource state.
		/// </summary>
		/// <param name="resourceState">The new resource state.</param>
		/// <param name="cmdList">The command list.</param>
		void Transition(D3D12_RESOURCE_STATES resourceState, ID3D12GraphicsCommandList* cmdList);

		/// <summary>
		/// Sets the resource state.
		/// </summary>
		/// <param name="resourceState">The resource state.</param>
		void SetResourceState(D3D12_RESOURCE_STATES resourceState);

		void Map(void** ptr, UINT subResource = 0, bool cpuRead = false);

		/// <summary>
		/// Gets the ID3D12Resource buffer.
		/// </summary>
		/// <returns>A pointer to the ID3D12Resource. Returns nullptr if no buffer has been created.</returns>
		_NODISCARD ID3D12Resource* GetBuffer() noexcept;

		_NODISCARD ID3D12Resource** GetBufferPtr() noexcept;
		//_NODISCARD ID3D12Resource** operator&() noexcept;

		/// <summary>
		/// Checks if a buffer has been created.
		/// </summary>
		/// <returns>true if a buffer has been created, otherwise false.</returns>
		_NODISCARD bool HasBuffer() noexcept;

		/// <summary>
		/// Gets the current resource state.
		/// </summary>
		/// <returns>The current resource state.</returns>
		_NODISCARD D3D12_RESOURCE_STATES GetResourceState() noexcept;

		/// <summary>
		/// Gets the buffer's GPU virtual address.
		/// </summary>
		/// <returns>The buffer's GPU virtual address.</returns>
		_NODISCARD D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress();

		/// <summary>
		/// Releases the heap, called automatically at destruction.
		/// </summary>
		void Release();

		~Heap();

	};

}