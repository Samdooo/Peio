#pragma once

#include "Global.h"

namespace Peio::Gfx {

	//struct PEIO_GFX_EXPORT PipelineState {
	//
	//	static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDesc(D3D12_INPUT_LAYOUT_DESC inputLayout, ID3D12RootSignature* rootSignature
	//		, D3D12_SHADER_BYTECODE vertexShader, D3D12_SHADER_BYTECODE pixelShader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	//
	//	static ID3D12PipelineState* Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);
	//
	//};

	struct PEIO_GFX_EXPORT PipelineState {

		void Init(D3D12_INPUT_LAYOUT_DESC inputLayout, ID3D12RootSignature* rootSignature,
			D3D12_SHADER_BYTECODE vertexShader, D3D12_SHADER_BYTECODE pixelShader, bool useBlend = true, bool useDepth = false, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		void Set(ID3D12GraphicsCommandList* cmdList);

	protected:

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;

	};

}