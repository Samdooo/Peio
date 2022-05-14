#pragma once

#include "VoxelsHeader.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\InputLayout.h"
#include "..\Graphics\PipelineStateHeader.h"
#include "..\Graphics\RootSignature.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT DenoiseRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, Gfx::RootSignature* rootSignature, Float2 size);
		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect);

	protected:

		struct InputVertex {
			Float2 position = {};
			Float2 texCoord = {};
			Float2 size = {};
		};
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};

		Gfx::RootSignature* rootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;

	};

}