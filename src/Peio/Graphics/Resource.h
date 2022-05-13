#pragma once

#include "Heap.h"

namespace Peio::Gfx {

	/// <summary>
	/// A GPU buffer. A combination of a default and an upload heap.
	/// </summary>
	class PEIO_GFX_EXPORT Resource : public Heap {

	protected:

		using Heap::Init;
		using Heap::Release;

		Heap uploadHeap = {};

	public:

		void Init(D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false);
		void InitBuffer(UINT64 size, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false);

		void Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

		void Release();

		~Resource();

	};

}