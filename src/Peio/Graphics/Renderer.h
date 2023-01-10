#pragma once

#include "InputParameter.h"
#include "VertexLayout.h"
#include "Shader.h"
#include "VertexBuffer.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Renderer {

		void Create();

		void Render(ID3D12GraphicsCommandList* cmdList, VertexBuffer* vertexBuffer, D3D12_VIEWPORT viewPort, RECT scissorRect);

		VertexLayout vertexLayout = {};
		std::vector<InputParameter*> parameters = {};
		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

		Shader vs = {}, ps = {}, ds = {}, hs = {}, gs = {};

		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	protected:

		ComPtr<ID3D12PipelineState> pipelineState = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;

		std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};

	};

}