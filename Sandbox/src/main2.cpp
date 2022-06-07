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
#include <Peio/GUI/Text.h>
#include <Peio/Windows/TextListener.h>
#include <Peio/Offset.h>
#include <Peio/Windows/MouseListener.h>

#include <Peio/Media/Images.h>
#include <Peio/Clock.h>

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
		Peio::GUI::Rectangle::Init();

		Peio::Uint2 windowSize = { 1280, 720 };
		Peio::Win::Window window;
		window.CreateClass("Sandbox class1", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_POPUP | WS_VISIBLE, 0, {CW_USEDEFAULT, CW_USEDEFAULT}, windowSize);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		Peio::GUI::Texture texture;
		texture.Init(&graphics, Peio::Med::Images::Load("pic.png", AV_PIX_FMT_RGBA, { 600, 600 }, SWS_SPLINE), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();

		Peio::GUI::Rectangle rect;
		rect.Init(&graphics, { 0, 0 }, { 600, 600 });
		rect.SetTexture(&texture);
		rect.Upload();

		Peio::GUI::Font font;
		font.Init(&graphics, "Roboto-Light.ttf", 40);
		font.LoadLetters();
		font.LoadTextures();

		Peio::Offset<float, 2> mousePos = {};

		Peio::GUI::Text text;
		text.Init(&graphics, { 100, 100 }, {500, 300});
		text.position.parent = &mousePos;
		text.SetFont(&font);
		text.SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		text.SetSpaceWidth(15.0f);
		text.SetLineOffset(50.0f);

		text.Upload();

		Peio::Win::TextListener textListener;
		Peio::Win::Input::AddListener(&textListener);

		Peio::Win::MouseListener mouseListener;
		Peio::Win::Input::AddListener(&mouseListener);

		Peio::FunctionHandler<Peio::Win::TextEvent> textHandler(
			[&text](Peio::Win::TextEvent& event) {
				if (event.character == VK_BACK) {
					if (text.GetString().size()) {
						text.SetString(text.GetString().substr(0, text.GetString().size() - 1));
					}
					return;
				}
				if (event.character == VK_RETURN) {
					text.SetString(text.GetString() + '\n');
					return;
				}
				text.SetString(text.GetString() + event.character);
			}
		);

		Peio::Win::Input::AddEventHandler(&textHandler);

		Peio::FunctionHandler<Peio::Win::MouseMoveEvent> mouseHandler(
			[&mousePos](Peio::Win::MouseMoveEvent& event) {
				mousePos = (Peio::Float2)event.position;
			}
		);

		Peio::Win::Input::AddEventHandler(&mouseHandler);

		while (true) {
			window.HandleMessages();

			graphics.Clear({});
			 
			rect.Draw();
			text.Draw();

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