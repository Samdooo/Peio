#define PEIO_GFX_EXPORTING
#include "PipelineState.h"

D3D12_GRAPHICS_PIPELINE_STATE_DESC Peio::Gfx::PipelineState::CreateDesc(D3D12_INPUT_LAYOUT_DESC inputLayout, ID3D12RootSignature* rootSignature, D3D12_SHADER_BYTECODE vertexShader, D3D12_SHADER_BYTECODE pixelShader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};

	desc.InputLayout = inputLayout;
	desc.pRootSignature = rootSignature;
	desc.VS = vertexShader;
	desc.PS = pixelShader;
	desc.PrimitiveTopologyType = topologyType;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc = { 1, 0 };
	desc.SampleMask = 0xffffffff;
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.NumRenderTargets = 1;
	//desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	D3D12_BLEND_DESC blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	desc.BlendState = blendDesc;

	//D3D12_DEPTH_STENCIL_DESC dsDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//dsDesc.StencilEnable = false;
	//
	//desc.DepthStencilState = dsDesc;
	//desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	return desc;
}

ID3D12PipelineState* Peio::Gfx::PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
	HRESULT ret;

	ID3D12PipelineState* pso = nullptr;
	ret = device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), reinterpret_cast<void**>(&pso));
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create pipeline state object.", ret);
	}

	return pso;
}