#define PEIO_GFX_EXPORTING
#include "Renderer.h"

namespace Peio::Gfx {

	void Renderer::Create()
	{
		HRESULT ret = 0;

		rootParameters.resize(parameters.size());
		for (UINT i = 0; i < parameters.size(); i++) {
			rootParameters[i] = parameters[i]->CreateParameter();
		}

		CD3DX12_ROOT_SIGNATURE_DESC rootDesc = {};
		rootDesc.Init((UINT)rootParameters.size(), &rootParameters[0], (UINT)staticSamplers.size(), &staticSamplers[0], D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		if (rootSignature)
			rootSignature.ReleaseAndGetAddressOf();

		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ret = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to serialize root signature.", ret);
		}

		ret = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), &rootSignature);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create root signature.", ret);
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
		desc.pRootSignature = rootSignature.Get();
		if (vs.data.size())
			desc.VS = { &vs.data[0], vs.data.size() };
		if (ps.data.size())
			desc.PS = { &ps.data[0], ps.data.size() };
		if (ds.data.size())
			desc.DS = { &ds.data[0], ds.data.size() };
		if (hs.data.size())
			desc.HS = { &hs.data[0], hs.data.size() };
		if (gs.data.size())
			desc.GS = { &gs.data[0], gs.data.size() };
		desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		desc.BlendState.RenderTarget[0].BlendEnable = false;
		desc.SampleMask = 0xffffffff;
		desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		desc.InputLayout = vertexLayout.CreateLayout();
		desc.PrimitiveTopologyType = topologyType;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = format;
		desc.SampleDesc = { 1, 0 };
		desc.DepthStencilState.DepthEnable = false;
		desc.DSVFormat = DXGI_FORMAT_UNKNOWN;

		ret = device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), &pipelineState);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create pipeline state.", ret);
		}
	}

	void Renderer::Render(ID3D12GraphicsCommandList* cmdList, VertexBuffer* vertexBuffer, D3D12_VIEWPORT viewPort, RECT scissorRect)
	{
		cmdList->SetGraphicsRootSignature(rootSignature.Get());
		cmdList->SetPipelineState(pipelineState.Get());
		cmdList->RSSetViewports(1, &viewPort);
		cmdList->RSSetScissorRects(1, &scissorRect);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vertexBuffer->GetView());

		for (UINT i = 0; i < parameters.size(); i++) {
			parameters[i]->parameterIndex = i;
			parameters[i]->Set(cmdList);
		}

		cmdList->DrawInstanced(vertexBuffer->GetNumElements(), 1, 0, 0);
	}

}