#define PEIO_VXL_EXPORTING
#include "VoxelRenderer.h"

namespace Peio::Vxl {

	void VoxelRenderer::Init(ID3D12GraphicsCommandList* cmdList, Gfx::ShaderResourceView* srv, Float3 cameraPosition, Float2 cameraRotation, float fov, float aspectRatio)
	{
		vertexBuffer.Allocate(6);
		for (UINT i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }, cameraPosition, cameraRotation, fov, aspectRatio
			};
		}
		vertexBuffer.Upload(cmdList);

		this->srv = srv;

		std::vector<D3D12_ROOT_PARAMETER> rootParams(srv->GetNumResources());
		for (UINT i = 0; i < srv->GetNumResources(); i++)
			rootParams[i] = Gfx::RootParameter::CreateShaderResourceView(i, D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature = Gfx::RootSignature::Create(rootParams, {},
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);


		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = Gfx::PipelineState::CreateDesc(
			Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("CAMERA_POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
				Gfx::InputElement::Create("ROTATION", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("FOV", DXGI_FORMAT_R32_FLOAT),
				Gfx::InputElement::Create("ASPECT_RATIO", DXGI_FORMAT_R32_FLOAT),
				}),
				rootSignature.Get(), Gfx::Shader::Load("../bin/VoxelShaders/VoxelVS.cso"), Gfx::Shader::Load("../bin/VoxelShaders/VoxelPS.cso")
				);
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;

		pipelineState = Gfx::PipelineState::Create(pipelineDesc);
	}

	void VoxelRenderer::SetCameraPosition(Float3 position)
	{
		for (UINT i = 0; i < 6; i++)
			vertexBuffer.GetSubresourceBuffer()[i].cameraPosition = position;
	}

	void VoxelRenderer::SetCameraRotation(Float2 rotation)
	{
		for (UINT i = 0; i < 6; i++)
			vertexBuffer.GetSubresourceBuffer()[i].rotation = rotation;
	}

	void VoxelRenderer::SetFOV(float fov)
	{
		for (UINT i = 0; i < 6; i++)
			vertexBuffer.GetSubresourceBuffer()[i].fov = fov;
	}

	void VoxelRenderer::SetAspectRatio(float aspectRatio)
	{
		for (UINT i = 0; i < 6; i++)
			vertexBuffer.GetSubresourceBuffer()[i].aspectRatio = aspectRatio;
	}

	void VoxelRenderer::UpdateCamera(ID3D12GraphicsCommandList* cmdList)
	{
		vertexBuffer.Upload(cmdList);
	}

	void VoxelRenderer::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect)
	{
		cmdList->SetGraphicsRootSignature(rootSignature.Get());
		cmdList->SetPipelineState(pipelineState.Get());

		cmdList->RSSetViewports(1, &viewPort);
		cmdList->RSSetScissorRects(1, &scissorRect);

		for (UINT i = 0; i < srv->GetNumResources(); i++)
			cmdList->SetGraphicsRootShaderResourceView(i, srv->GetResources()[i].GetGPUAddress());

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
		cmdList->DrawInstanced(6, 1, 0, 0);
	}

}