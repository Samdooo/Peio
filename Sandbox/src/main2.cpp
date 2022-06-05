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
#include <Peio/GUI/Rectangle.h>

#include <Peio/Media/Images.h>

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

		Peio::Uint2 windowSize = { 1280, 720 };
		Peio::Win::Window window;
		window.CreateClass("Sandbox class2", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { CW_USEDEFAULT, CW_USEDEFAULT }, windowSize);
		window.Show();

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		Peio::GUI::Rectangle rect;
		rect.Init(&graphics, { 50, 50 }, { 600, 600 });

		Peio::GUI::Texture texture;
		texture.Init(&graphics, Peio::Med::Images::Load("pic.png", AV_PIX_FMT_RGBA, { 600, 600 }, SWS_SPLINE), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();

		rect.SetTexture(&texture);

		Peio::GUI::Texture texture2;
		texture2.Init(&graphics, Peio::Med::Images::Load("s.png", AV_PIX_FMT_RGBA), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture2.Upload();

		rect.SetAlphaTexture(&texture2);

		rect.Upload();

		while (true) {
			window.HandleMessages();

			graphics.Clear({});

			rect.Draw();

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