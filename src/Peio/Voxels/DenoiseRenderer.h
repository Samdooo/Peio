#pragma once

#include "VoxelsHeader.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\InputLayout.h"
#include "..\Graphics\PipelineStateHeader.h"
#include "..\Graphics\RootSignature.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT DenoiseRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, UINT numSrvs, UINT numUavs, Float2 size);
		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect);

		Gfx::PipelineState pipelineState = {};
	protected:

		struct InputVertex {
			Float2 position = {};
			Float2 texCoord = {};
			Float2 size = {};
		};
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};

	};

}