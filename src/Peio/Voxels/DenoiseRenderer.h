#pragma once

#include "VertexBuffer.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\InputLayout.h"
#include "..\Graphics\PipelineStateHeader.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT DenoiseRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, Float2 size);
		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect, ID3D12DescriptorHeap* heap, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);

	protected:

		struct InputVertex {
			Float2 position = {};
			Float2 texCoord = {};
			Float2 size = {};
		};
		VertexBuffer<InputVertex> vertexBuffer = {};

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;

	};

}