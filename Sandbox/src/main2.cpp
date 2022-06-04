#include <iostream>
#include <unordered_map>

#include <Peio/Media/FileDecoder.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Windows/Window.h>
#include <Peio/Graphics/RootSignature.h>
#include <Peio/Graphics/RootParameter.h>
#include <Peio/Graphics/ConstantBufferView.h>
#include <Peio/Graphics/SubresourceBuffer.h>
#include <Peio/Graphics/VertexBuffer.h>
#include <Peio/Graphics/PipelineState.h>
#include <Peio/Graphics/InputLayout.h>
#include <Peio/Graphics/Shader.h>
#include <Peio/Graphics/Sampler.h>
#include <Peio/Graphics/DescriptorTable.h>
#include <Peio/Graphics/ShaderResourceView.h>

#include <Peio/Media/FileDecoder.h>

struct Rect {
	Peio::Uint2 offset;
	Peio::Uint2 position;
	Peio::Uint2 size;
	BOOL useColor, useTexture, useAlpha;
};

struct VSInput {
	Peio::Float2 position;
	Peio::Float4 color;
	Peio::Float2 texCoord;
	Peio::Float2 alphaCoord;
};

int main() {

	try {

		Peio::Gfx::Init();

		std::vector<D3D12_ROOT_PARAMETER> rootParams = {
			Peio::Gfx::RootParameter::CreateConstantBufferView(0, D3D12_SHADER_VISIBILITY_VERTEX),
			Peio::Gfx::RootParameter::CreateConstantBufferView(1, D3D12_SHADER_VISIBILITY_VERTEX),
			Peio::Gfx::RootParameter::CreateDescriptorTable(Peio::Gfx::DescriptorTable::Create({ Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0) }), D3D12_SHADER_VISIBILITY_PIXEL)
			//Peio::Gfx::RootParameter::CreateShaderResourceView(0, D3D12_SHADER_VISIBILITY_PIXEL)
			//Peio::Gfx::RootParameter::CreateShaderResourceView(1, D3D12_SHADER_VISIBILITY_PIXEL)
		};

		std::vector<D3D12_STATIC_SAMPLER_DESC> samplers = {
			Peio::Gfx::Sampler::Create(0),
			Peio::Gfx::Sampler::Create(1)
		};

		ID3D12RootSignature* rootSignature = Peio::Gfx::RootSignature::Create(rootParams, samplers,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = Peio::Gfx::PipelineState::CreateDesc(
			Peio::Gfx::InputLayout::Create({
				Peio::Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Peio::Gfx::InputElement::Create("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT),
				Peio::Gfx::InputElement::Create("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT),
				Peio::Gfx::InputElement::Create("ALPHACOORD", DXGI_FORMAT_R32G32_FLOAT)
				}),
			rootSignature,
			Peio::Gfx::Shader::Load("TestVS.cso"), Peio::Gfx::Shader::Load("TestPS.cso")
		);
		pipelineDesc.BlendState.IndependentBlendEnable = FALSE;
		pipelineDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;

		ID3D12PipelineState* pipeline = Peio::Gfx::PipelineState::Create(pipelineDesc);

		Peio::Uint2 windowSize = { 1280, 720 };
		Peio::Win::Window window;
		window.CreateClass("Sandbox class", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { CW_USEDEFAULT, CW_USEDEFAULT }, windowSize);
		window.Show();

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		Peio::Gfx::ConstantBufferView windowBuffer;
		windowBuffer.Init(sizeof(windowSize), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		Rect rect = {
			{ 15, 15 }, { 10, 10 }, { 400, 300 }, true, true, false
		};
		Peio::Gfx::ConstantBufferView rectBuffer;
		rectBuffer.Init(sizeof(rect), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		Peio::Gfx::ResourceArray resourceArray;
		resourceArray.AddResource(&windowBuffer);
		resourceArray.AddResource(&rectBuffer);
		resourceArray.CreateResources();

		Peio::Gfx::SubresourceBuffer<Peio::Uint2> windowData;
		windowData.SetBuffer(&windowSize, 1);
		windowBuffer.Upload(windowData.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Rect> rectData;
		rectData.SetBuffer(&rect, 1);
		rectBuffer.Upload(rectData.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::VertexBuffer<VSInput> vertexBuffer;
		vertexBuffer.Allocate(6);
		for (int i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = {
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) }, { 0.1f, 0.5f, 0.8f, 1.0f },
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) },
				{ (float)(i >= 1 && i <= 3), (float)(i >= 2 && i <= 4) }
			};
			std::cout << vertexBuffer.GetSubresourceBuffer()[i].position.ToString() << std::endl;
		}
		vertexBuffer.Upload(graphics.GetCommandList());

		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, windowSize.x(), windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };

		Peio::Med::FileDecoder decoder;
		decoder.Init("pic.png");
		Peio::Med::Frame frame;
		frame.Init(nullptr, {}, AV_PIX_FMT_RGBA);
		decoder.DecodeFrame(&frame, true);
		decoder.Release();

		Peio::Gfx::Texture texture;
		texture.Init(frame.GetSize(), DXGI_FORMAT_R8G8B8A8_UNORM);

		Peio::Gfx::ResourceArray textureArray;
		textureArray.AddResource(&texture);
		textureArray.CreateResources();

		Peio::Gfx::SubresourceBuffer<Peio::byte> textureData;
		textureData.SetBuffer(frame.GetData(), frame.GetBytesPP() * frame.GetSize().x(), frame.GetSize().y());
		texture.Upload(textureData.GetResourceData(), graphics.GetCommandList());

		while (true) {
			window.HandleMessages();

			graphics.Clear({});
			
			graphics.GetCommandList()->SetGraphicsRootSignature(rootSignature);
			graphics.GetCommandList()->SetPipelineState(pipeline);

			graphics.GetCommandList()->RSSetScissorRects(1, &scissorRect);
			graphics.GetCommandList()->RSSetViewports(1, &viewPort);
			
			graphics.GetCommandList()->SetGraphicsRootConstantBufferView(0, windowBuffer.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootConstantBufferView(1, rectBuffer.GetGPUAddress());

			graphics.GetCommandList()->SetDescriptorHeaps(1, textureArray.GetDescriptorHeaps());
			graphics.GetCommandList()->SetGraphicsRootDescriptorTable(2, textureArray.GetDescriptorHeap().GetGPUHandle());

			graphics.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			graphics.GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
			graphics.GetCommandList()->DrawInstanced(6, 1, 0, 0);

			graphics.Render();
		}


	}
	catch (Peio::Gfx::Exception e) {
		std::cout << "GfxException: " << e.msg << " at " << e.file << " line " << e.line << std::endl;
	}
	catch (Peio::Exception e) {
		std::cout << "Exception: " << e.msg << " at " << e.file << " line " << e.line << std::endl;
	}

	return 0;
}