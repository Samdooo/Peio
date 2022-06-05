#define PEIO_GUI_EXPORTING
#include "Rectangle.h"

namespace Peio::GUI {

	void Rectangle::Init(Gfx::Graphics* graphics, Uint2 position, Uint2 size, Uint2 offset)
	{
		this->graphics = graphics;

		pso.Init(Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT),
				Gfx::InputElement::Create("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("ALPHACOORD", DXGI_FORMAT_R32G32_FLOAT)
			}), Gfx::RootSignature::Create({
				Gfx::RootParameter::CreateConstantBufferView(0, D3D12_SHADER_VISIBILITY_VERTEX),
				Gfx::RootParameter::CreateConstantBufferView(1, D3D12_SHADER_VISIBILITY_VERTEX),
				Gfx::RootParameter::CreateDescriptorTable(Peio::Gfx::DescriptorTable::Create({ Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0) }), D3D12_SHADER_VISIBILITY_PIXEL),
				Gfx::RootParameter::CreateDescriptorTable(Peio::Gfx::DescriptorTable::Create({ Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1) }), D3D12_SHADER_VISIBILITY_PIXEL)
			}, {
				Gfx::Sampler::Create(0),
				Gfx::Sampler::Create(1)
			}, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS), Gfx::Shader::Load("../bin/GUIShaders/RectVS.cso"), Gfx::Shader::Load("../bin/GUIShaders/RectPS.cso"));

		vertices.Allocate(6);

		for (size_t i = 0; i < 6; i++) {
			vertices[i] = {
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) }, {},
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) },
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) }
			};
		}

		rect.Allocate(1);
		rectWindow.Allocate(1);

		rect[0].offset = offset;
		rect[0].position = position;
		rect[0].size = size;

		rectWindow[0].size = (Uint2)graphics->GetSize();

		rectBuffer.Init(sizeof(rect), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		rectWindowBuffer.Init(sizeof(rectWindow), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		cbvArray.AddResource(&rectBuffer);
		cbvArray.AddResource(&rectWindowBuffer);
		cbvArray.CreateResources();

		viewPort = { 0.0f, 0.0f, (float)graphics->GetSize().x(), (float)graphics->GetSize().y(), 0.0f, 1.0f};
		scissorRect = { 0, 0, graphics->GetSize().x(), graphics->GetSize().y() };
	}

	void Rectangle::SetColor(Float4 color)
	{
		for (size_t i = 0; i < 6; i++)
			vertices[i].color = color;
		rect[0].useColor = true;
	}

	void Rectangle::SetTexture(Texture* texture)
	{
		this->texture = texture;
		descriptorHeaps[0] = texture->GetResourceArray().GetDescriptorHeap().GetHeap();
		rect[0].useTexture = true;
	}

	void Rectangle::SetAlphaTexture(Texture* alphaTexture)
	{
		this->alphaTexture = alphaTexture;
		descriptorHeaps[1] = alphaTexture->GetResourceArray().GetDescriptorHeap().GetHeap();
		rect[0].useAlpha = true;
	}

	void Rectangle::Upload()
	{
		vertices.Upload(graphics->GetCommandList());
		rectBuffer.Upload(rect, graphics->GetCommandList());
		rectWindowBuffer.Upload(rectWindow, graphics->GetCommandList());
	}

	void Rectangle::Draw()
	{
		pso.Set(graphics->GetCommandList());

		graphics->GetCommandList()->RSSetScissorRects(1, &scissorRect);
		graphics->GetCommandList()->RSSetViewports(1, &viewPort);

		graphics->GetCommandList()->SetGraphicsRootConstantBufferView(0, rectWindowBuffer.GetGPUAddress());
		graphics->GetCommandList()->SetGraphicsRootConstantBufferView(1, rectBuffer.GetGPUAddress());
		
		if (texture) {
			graphics->GetCommandList()->SetDescriptorHeaps(1, texture->GetResourceArray().GetDescriptorHeaps());
			graphics->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture->GetResourceArray().GetDescriptorHeap().GetGPUHandle());
		}
		if (alphaTexture) {
			graphics->GetCommandList()->SetDescriptorHeaps(1, alphaTexture->GetResourceArray().GetDescriptorHeaps());
			graphics->GetCommandList()->SetGraphicsRootDescriptorTable(3, alphaTexture->GetResourceArray().GetDescriptorHeap().GetGPUHandle());
		}

		graphics->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphics->GetCommandList()->IASetVertexBuffers(0, 1, &vertices.GetBufferView());
		graphics->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	}

	Rect& Rectangle::GetRect() const noexcept
	{
		return rect[0];
	}

}