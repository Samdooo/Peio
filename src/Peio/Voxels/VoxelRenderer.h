#pragma once

#include "VoxelsHeader.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\RootSignature.h"
#include "..\Graphics\InputLayout.h"
#include "..\Graphics\PipelineStateHeader.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT VoxelRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, UINT numSrvs, UINT numUavs, Float3 cameraPosition, Float2 cameraRotation, float fov, float aspectRatio);

		void SetCameraPosition(Float3 position);
		void SetCameraRotation(Float2 rotation);
		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void UpdateCamera(ID3D12GraphicsCommandList* cmdList);

		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect);

		Gfx::PipelineState pipelineState = {};
	protected:

		struct InputVertex {
			Float2 position = {};
			Float3 cameraPosition = {};
			Float2 rotation = {};
			float fov = 1.5f;
			float aspectRatio = 1.0f;
		};
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};

	};

}