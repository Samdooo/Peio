#pragma once

#include "VertexBuffer.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\InputLayout.h"
#include "..\Graphics\PipelineStateHeader.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT VoxelRenderer {

		void Init(ID3D12GraphicsCommandList* cmdList, Gfx::ShaderResourceView* srv, Float3 cameraPosition, Float2 cameraRotation, float fov, float aspectRatio);

		void SetCameraPosition(Float3 position);
		void SetCameraRotation(Float2 rotation);
		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void UpdateCamera(ID3D12GraphicsCommandList* cmdList);

		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect);

	protected:

		struct InputVertex {
			Float2 position = {};
			Float3 cameraPosition = {};
			Float2 rotation = {};
			float fov = 1.5f;
			float aspectRatio = 1.0f;
		};
		VertexBuffer<InputVertex> vertexBuffer = {};

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;

		Gfx::ShaderResourceView* srv = nullptr;

	};

}