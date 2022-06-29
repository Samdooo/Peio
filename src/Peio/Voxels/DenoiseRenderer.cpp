#define PEIO_VXL_EXPORTING
#include "DenoiseRenderer.h"

namespace Peio::Vxl {

	void DenoiseRenderer::Init(ID3D12GraphicsCommandList* cmdList, UINT numSrvs, UINT numUavs, Float2 size)
	{
		vertexBuffer.Allocate(6);
		for (UINT i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }, 
				{ (i >= 1 && i <= 3) ? size.x() : 0.0f, (i >= 2 && i <= 4) ? size.y() : 0.0f},
				size
			};
		}
		vertexBuffer.Upload(cmdList);

		std::vector<D3D12_ROOT_PARAMETER> rootParams(numSrvs + numUavs);
		for (UINT i = 0; i < numSrvs; i++)
			rootParams[i] = Gfx::RootParameter::CreateShaderResourceView(i, D3D12_SHADER_VISIBILITY_PIXEL);
		for (UINT i = 0; i < numUavs; i++)
			rootParams[numSrvs + i] = Gfx::RootParameter::CreateUnorderedAccessView(i + 1, D3D12_SHADER_VISIBILITY_PIXEL);

		ID3D12RootSignature* rootSignature = Gfx::RootSignature::Create(rootParams, {},
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		pipelineState.Init(
			Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("SIZE", DXGI_FORMAT_R32G32_FLOAT)
				}), rootSignature, Gfx::Shader::Load("../bin/VoxelShaders/DenoiseVS.cso"),
				Gfx::Shader::Load("../bin/VoxelShaders/DenoisePS.cso"), false
				);

		//D3D12_STATIC_SAMPLER_DESC sampler;
		//sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		//sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		//sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		//sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		//sampler.MipLODBias = 0;
		//sampler.MaxAnisotropy = 0;
		//sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		//sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		//sampler.MinLOD = 0.0f;
		//sampler.MaxLOD = D3D12_FLOAT32_MAX;
		//sampler.ShaderRegister = 0;
		//sampler.RegisterSpace = 0;
		//sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		//
		//rootSignature = Gfx::RootSignature::Create({
		//	Gfx::RootParameter::CreateDescriptorTable(Gfx::DescriptorTable::Create({
		//		Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0)
		//	}), D3D12_SHADER_VISIBILITY_PIXEL)
		//	}, {
		//		sampler
		//	}, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		//	D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);
		//
		//D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = Gfx::PipelineState::CreateDesc(
		//	Gfx::InputLayout::Create({
		//		Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
		//		Gfx::InputElement::Create("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
		//		Gfx::InputElement::Create("SIZE", DXGI_FORMAT_R32G32_FLOAT)
		//		}),
		//	rootSignature->GetRootSignature(), Gfx::Shader::Load("../bin/VoxelShaders/DenoiseVS.cso"), Gfx::Shader::Load("../bin/VoxelShaders/DenoisePS.cso")
		//);
		//pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		//pipelineDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
		//
		//pipelineState = Gfx::PipelineState::Create(pipelineDesc);
	}

	void DenoiseRenderer::Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect)
	{
		//rootSignature->SetRootSignature(cmdList);
		//cmdList->SetPipelineState(pipelineState.Get());
		//pipelineState.Set(cmdList);

		cmdList->RSSetViewports(1, &viewPort);
		cmdList->RSSetScissorRects(1, &scissorRect);

		//ID3D12DescriptorHeap* heaps[1] = { heap };
		//cmdList->SetDescriptorHeaps(1, heaps);
		//cmdList->SetGraphicsRootDescriptorTable(0, gpuHandle);

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
		cmdList->DrawInstanced(6, 1, 0, 0);
	}

}