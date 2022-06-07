#define PEIO_GUI_EXPORTING
#include "Rectangle.h"

namespace Peio::GUI {

	Gfx::PipelineState Rectangle::pipelineState = {};

	void Rectangle::Init()
	{
		pipelineState.Init(Gfx::InputLayout::Create({
				Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT),
				Gfx::InputElement::Create("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
				Gfx::InputElement::Create("ALPHACOORD", DXGI_FORMAT_R32G32_FLOAT)
			}), Gfx::RootSignature::Create({
				Gfx::RootParameter::CreateConstantBufferView(0, D3D12_SHADER_VISIBILITY_PIXEL),
				Gfx::RootParameter::CreateDescriptorTable(Peio::Gfx::DescriptorTable::Create({ Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0) }), D3D12_SHADER_VISIBILITY_PIXEL),
				Gfx::RootParameter::CreateDescriptorTable(Peio::Gfx::DescriptorTable::Create({ Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1) }), D3D12_SHADER_VISIBILITY_PIXEL)
				}, {
					Gfx::Sampler::Create(0),
					Gfx::Sampler::Create(1)
				}, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS), Gfx::Shader::Load("../bin/GUIShaders/RectVS.cso"), Gfx::Shader::Load("../bin/GUIShaders/RectPS.cso"));
	}

	void Rectangle::Init(Gfx::Graphics* graphics, Offset<float, 2> position, Float2 size)
	{
		this->graphics = graphics;

		vertices.Allocate(6);
		
		for (size_t i = 0; i < 6; i++) {
			vertices[i] = {
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }, {},
				{ (i >= 1 && i <= 3) ? 1.0f : 0.0f, (i >= 2 && i <= 4) ? 1.0f : 0.0f },
				{ (i >= 1 && i <= 3) ? 1.0f : 0.0f, (i >= 2 && i <= 4) ? 1.0f : 0.0f }
			};
		}
		
		this->position = position;
		this->size = size;
		//viewPort = { position.x(), position.y(), size.x(), size.y(), 0.0f, 1.0f };

		info.Allocate(1);
		infoBuffer.Init(sizeof(info), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		scissorRect = { 0, 0, graphics->GetSize().x(), graphics->GetSize().y() };
	}

	void Rectangle::SetColor(Float4 color)
	{
		for (size_t i = 0; i < 6; i++)
			vertices[i].color = color;
		info[0].useColor = true;
	}

	void Rectangle::SetTexture(const Texture* texture)
	{
		this->texture = texture;
		info[0].useTexture = true;
	}

	void Rectangle::SetAlphaTexture(const Texture* alphaTexture)
	{
		this->alphaTexture = alphaTexture;
		info[0].useAlpha = true;
	}

	void Rectangle::Upload()
	{
		vertices.Upload(graphics->GetCommandList());
		infoBuffer.Upload(info, graphics->GetCommandList());
	}

	void Rectangle::Draw()
	{
		Float2 translated = position.GetTranslated();
		viewPort.TopLeftX = translated.x();
		viewPort.TopLeftY = translated.y();

		Rectangle::pipelineState.Set(graphics->GetCommandList());
		
		graphics->GetCommandList()->RSSetScissorRects(1, &scissorRect);
		graphics->GetCommandList()->RSSetViewports(1, &viewPort);

		graphics->GetCommandList()->SetGraphicsRootConstantBufferView(0, infoBuffer.GetGPUAddress());
		
		if (info[0].useTexture) {
			graphics->GetCommandList()->SetDescriptorHeaps(1, texture->GetResourceArray().GetDescriptorHeaps());
			graphics->GetCommandList()->SetGraphicsRootDescriptorTable(1, texture->GetResourceArray().GetDescriptorHeap().GetGPUHandle());
		}
		if (info[0].useAlpha) {
			graphics->GetCommandList()->SetDescriptorHeaps(1, alphaTexture->GetResourceArray().GetDescriptorHeaps());
			graphics->GetCommandList()->SetGraphicsRootDescriptorTable(2, alphaTexture->GetResourceArray().GetDescriptorHeap().GetGPUHandle());
		}
		
		graphics->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphics->GetCommandList()->IASetVertexBuffers(0, 1, &vertices.GetBufferView());
		graphics->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	}

	//Float2& Rectangle::GetPosition() noexcept
	//{
	//	return *reinterpret_cast<Float2*>(&viewPort.TopLeftX);
	//}
	//
	//Float2& Rectangle::GetSize() noexcept
	//{
	//	return *reinterpret_cast<Float2*>(&viewPort.Width);
	//}

	RectangleInfo& Rectangle::GetInfo() const noexcept
	{
		return info[0];
	}

}