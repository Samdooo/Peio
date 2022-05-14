#pragma once

#include "SubresourceBuffer.h"
#include "Resource.h"

namespace Peio::Gfx {

	template <typename T_vertex>
	struct VertexBuffer : public Resource, public SubresourceBuffer<T_vertex> {

		_NODISCARD const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() const noexcept {
			return bufferView;
		}

		void Allocate(size_t n_elements) {
			SubresourceBuffer<T_vertex>::Allocate(n_elements);
			Gfx::Resource::Init(CD3DX12_RESOURCE_DESC::Buffer(n_elements * sizeof(T_vertex)), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			bufferView.BufferLocation = Gfx::Resource::GetGPUAddress();
			bufferView.StrideInBytes = sizeof(T_vertex);
			bufferView.SizeInBytes = (UINT)(n_elements * sizeof(T_vertex));
		}

		void Upload(ID3D12GraphicsCommandList* cmdList) {
			Gfx::Resource::Upload(SubresourceBuffer<T_vertex>::resourceData, cmdList);
		}

	protected:

		D3D12_VERTEX_BUFFER_VIEW bufferView = {};

	};

}