#define PEIO_VXL_EXPORTING
#include "VoxelRenderer.h"

#include <iostream>

void Peio::Vxl::VoxelRenderer::Init(ID3D12GraphicsCommandList* cmdList)
{
	vertexBuffer.Allocate(6);
	for (uint i = 0; i < 6; i++) {
		vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
			{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }
		};
	}
	vertexBuffer.Upload(cmdList);

	std::vector<D3D12_ROOT_PARAMETER> rootParams = {
		Gfx::RootParameter::CreateShaderResourceView(0, D3D12_SHADER_VISIBILITY_VERTEX), // Camera buffer
		Gfx::RootParameter::CreateShaderResourceView(1, D3D12_SHADER_VISIBILITY_PIXEL), // Scene buffer
		Gfx::RootParameter::CreateShaderResourceView(2, D3D12_SHADER_VISIBILITY_PIXEL), // materialMap buffer
		Gfx::RootParameter::CreateShaderResourceView(3, D3D12_SHADER_VISIBILITY_PIXEL), // Material buffer
		Gfx::RootParameter::CreateUnorderedAccessView(1, D3D12_SHADER_VISIBILITY_PIXEL), // Ray buffer
	};

	ID3D12RootSignature* rootSignature = Gfx::RootSignature::Create(rootParams, {},
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

	pipelineState.Init(
		Gfx::InputLayout::Create({
			Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT)
			}), rootSignature, Gfx::Shader::Load("../bin/VoxelShaders/VoxelVS.cso"),
			Gfx::Shader::Load("../bin/VoxelShaders/VoxelPS.cso"), false
			);

	sceneBuffer.SetBuffer(&scene, 1);
	sceneSrv.Init(sizeof(Scene), 1, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	sceneSrv.Upload(sceneBuffer.GetResourceData(), cmdList);

	cameraBuffer.SetBuffer(&camera, 1);
	cameraSrv.Init(sizeof(Camera), 1, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
	cameraSrv.Upload(cameraBuffer.GetResourceData(), cmdList);

	materialMapBuffer.SetBuffer(&materialMap.nodes[0], materialMap.nodes.size());
	materialMapSrv.Init(sizeof(IndexMap<uint, 3>::Node) * materialMap.nodes.size(), materialMap.nodes.size(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	materialMapSrv.Upload(materialMapBuffer.GetResourceData(), cmdList);

	//rayUav.Init(sizeof(Ray) * (uint)scene.windowSize.x() * (uint)scene.windowSize.y(), (uint)scene.windowSize.x() * (uint)scene.windowSize.y(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void Peio::Vxl::VoxelRenderer::UpdateScene(ID3D12GraphicsCommandList* cmdList)
{
	sceneSrv.Upload(sceneBuffer.GetResourceData(), cmdList);
}

void Peio::Vxl::VoxelRenderer::UpdateCamera(ID3D12GraphicsCommandList* cmdList)
{
	cameraSrv.Upload(cameraBuffer.GetResourceData(), cmdList);
}

void Peio::Vxl::VoxelRenderer::UpdateMaterialMap(ID3D12GraphicsCommandList* cmdList)
{
	materialMapSrv.Upload(materialMapBuffer.GetResourceData(), cmdList);
}

void Peio::Vxl::VoxelRenderer::Render(ID3D12GraphicsCommandList* cmdList, D3D12_VIEWPORT viewPort, D3D12_RECT scissorRect, Gfx::BufferSRV* materialSrv, Gfx::BufferUAV* rayUav)
{
	pipelineState.Set(cmdList);
	cmdList->RSSetViewports(1, &viewPort);
	cmdList->RSSetScissorRects(1, &scissorRect);

	cmdList->SetGraphicsRootShaderResourceView(0, cameraSrv.GetGPUAddress());
	cmdList->SetGraphicsRootShaderResourceView(1, sceneSrv.GetGPUAddress());
	cmdList->SetGraphicsRootShaderResourceView(2, materialMapSrv.GetGPUAddress());
	cmdList->SetGraphicsRootShaderResourceView(3, materialSrv->GetGPUAddress());
	cmdList->SetGraphicsRootUnorderedAccessView(4, rayUav->GetGPUAddress());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
	cmdList->DrawInstanced(6, 1, 0, 0);
}

const Peio::Gfx::BufferSRV* Peio::Vxl::VoxelRenderer::GetSceneSrv() const
{
	return &sceneSrv;
}
