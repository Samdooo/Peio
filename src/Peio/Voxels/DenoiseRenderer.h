#pragma once

#include "VoxelRenderer.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT DenoiseRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList);

		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect, const Gfx::BufferSRV* sceneSrv, const Gfx::BufferSRV* materialSrv, const Gfx::BufferUAV* rayUav);

	protected:

		struct InputVertex {
			Float2 position = {};
		};
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};
		Gfx::PipelineState pipelineState = {};

	};

}