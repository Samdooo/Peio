#define PEIO_VXL_EXPORTING
#include "FullscreenRenderer.h"

namespace Peio::Vxl {

	void FullscreenRenderer::Init(ID3D12GraphicsCommandList* cmdList, const char* vertexShaderPath, const char* pixelShaderPath, 
		std::vector<D3D12_SHADER_VISIBILITY> srvs, std::vector<D3D12_SHADER_VISIBILITY> uavs)
	{
		numSrvs = srvs.size();
		numUavs = uavs.size();

		vertexBuffer.Allocate(6);
		for (uint i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }
			};
		}
		vertexBuffer.Upload(cmdList);

		std::vector<D3D12_ROOT_PARAMETER> rootParams;
		for (uint i = 0; i < srvs.size(); i++) {
			rootParams.push_back(Gfx::RootParameter::CreateShaderResourceView(i, srvs[i]));
		}
		for (uint i = 0; i < uavs.size(); i++) {
			rootParams.push_back(Gfx::RootParameter::CreateUnorderedAccessView(i + 1, uavs[i])); // UAV registers begin at index 1
		}

		ID3D12RootSignature* rootSignature = Gfx::RootSignature::Create(rootParams, {},
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		pipelineState.Init(
			Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT)
				}), rootSignature, Gfx::Shader::Load(vertexShaderPath), Gfx::Shader::Load(pixelShaderPath), false);
	}

	void FullscreenRenderer::Prepare(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect)
	{
		pipelineState.Set(cmdList);
		cmdList->RSSetViewports(1, &viewPort);
		cmdList->RSSetScissorRects(1, &scissorRect);
	}

	void FullscreenRenderer::SetSRV(ID3D12GraphicsCommandList* cmdList, UINT index, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		cmdList->SetGraphicsRootShaderResourceView(index, address);
	}

	void FullscreenRenderer::SetUAV(ID3D12GraphicsCommandList* cmdList, UINT index, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		cmdList->SetGraphicsRootUnorderedAccessView(numSrvs + index, address);
	}

	void FullscreenRenderer::Render(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
		cmdList->DrawInstanced(6, 1, 0, 0);
	}

}