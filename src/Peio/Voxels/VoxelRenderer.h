#pragma once

#include "MaterialMap.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\PipelineStateHeader.h"
#include "..\Graphics\ConstantBufferView.h"
#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\UnorderedAccessView.h"
#include "Camera.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT VoxelRenderer {

		struct Scene {
			struct Sky {
				Float2 sunRotation = {};
			};
			uint numRays = 1;
			Uint2 windowSize = {};
			Sky sky = {};
			BOOL newRays = TRUE;
		};
		//struct Camera {
		//	Peio::Float3 position = {};
		//	Peio::Float2 rotation = {};
		//	float fov = 1.5f;
		//	float aspectRatio = 1.0f;
		//};
		struct Ray {
			Uint3 voxel = {};
			uint material = ~0;
			uint side = ~0;
			Float3 light = {};
			uint numRays = 0;
		};

		Scene scene = {};
		SmoothCamera camera = {};
		MaterialMap materialMap = {};

		void Init(ID3D12GraphicsCommandList* cmdList);
		
		void UpdateScene(ID3D12GraphicsCommandList* cmdList);
		void UpdateCamera(ID3D12GraphicsCommandList* cmdList);
		void UpdateMaterialMap(ID3D12GraphicsCommandList* cmdList);

		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect, Gfx::BufferSRV* materialSrv, Gfx::BufferUAV* rayUav, Gfx::BufferUAV* randomUav);

		_NODISCARD const Gfx::BufferSRV* GetSceneSrv() const;

	protected:

		struct InputVertex {
			Float2 position = {};
		};
		Gfx::VertexBuffer<InputVertex> vertexBuffer = {};
		Gfx::PipelineState pipelineState = {};

		Gfx::SubresourceBuffer<Scene> sceneBuffer = {};
		Gfx::BufferSRV sceneSrv = {};

		Gfx::SubresourceBuffer<Camera> cameraBuffer = {};
		Gfx::BufferSRV cameraSrv = {};

		Gfx::SubresourceBuffer<MaterialMap::Node> materialMapBuffer = {};
		Gfx::BufferSRV materialMapSrv = {};

		//Gfx::BufferUAV rayUav = {};

	};

}