#define PEIO_VXL_EXPORTING
#include "DenoiseRenderer.h"

namespace Peio::Vxl {

	void DenoiseRenderer::Init(ID3D12GraphicsCommandList* cmdList)
	{
		vertexBuffer.Allocate(6);
		for (uint i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }
			};
		}
		vertexBuffer.Upload(cmdList);

		std::vector<D3D12_ROOT_PARAMETER> rootParams = {
			Gfx::RootParameter::CreateShaderResourceView(0, D3D12_SHADER_VISIBILITY_PIXEL), // Scene buffer
			Gfx::RootParameter::CreateShaderResourceView(1, D3D12_SHADER_VISIBILITY_PIXEL), // Material buffer
			Gfx::RootParameter::CreateUnorderedAccessView(1, D3D12_SHADER_VISIBILITY_PIXEL) // Ray buffer
		};

		ID3D12RootSignature* rootSignature = Gfx::RootSignature::Create(rootParams, {},
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		pipelineState.Init(
			Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT)
				}), rootSignature, Gfx::Shader::Load("../bin/VoxelShaders/DenoiseVS.cso"),
			Gfx::Shader::Load("../bin/VoxelShaders/DenoisePS.cso"), false
		);
	}

	void DenoiseRenderer::Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect, const Gfx::BufferSRV* sceneSrv, const Gfx::BufferSRV* materialSrv, const Gfx::BufferUAV* rayUav)
	{
		pipelineState.Set(cmdList);
		cmdList->RSSetViewports(1, &viewPort);
		cmdList->RSSetScissorRects(1, &scissorRect);

		cmdList->SetGraphicsRootShaderResourceView(0, sceneSrv->GetGPUAddress());
		cmdList->SetGraphicsRootShaderResourceView(1, materialSrv->GetGPUAddress());
		cmdList->SetGraphicsRootShaderResourceView(2, rayUav->GetGPUAddress());

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
		cmdList->DrawInstanced(6, 1, 0, 0);
	}

}