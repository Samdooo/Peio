#pragma once

#include "VoxelsHeader.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\PipelineStateHeader.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT FullscreenRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, const char* vertexShaderPath, const char* pixelShaderPath, 
			std::vector<D3D12_SHADER_VISIBILITY> srvs, std::vector<D3D12_SHADER_VISIBILITY> uavs);

		void Prepare(ID3D12GraphicsCommandList* cmdList);
		void SetSRV(ID3D12GraphicsCommandList* cmdList, UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);
		void SetUAV(ID3D12GraphicsCommandList* cmdList, UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);
		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect);

	protected:

		struct InputVertex {
			Float2 position = {};
		};

		UINT numSrvs = 0, numUavs = 0;
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};
		Gfx::PipelineState pipelineState = {};

	};

}