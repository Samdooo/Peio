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
#include <Peio/GUI/Button.h>
#include <Peio/Windows/RawMouseListener.h>

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

struct Titlebar : public Peio::GUI::Button {

	Titlebar(Peio::Win::Window& window, Peio::Gfx::Graphics* graphics) : window(window) {
		texture.Init(graphics, Peio::Med::Images::Load("titleBar.png", AV_PIX_FMT_RGBA, { 1920, 25 }), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();
		Init(graphics, {}, { 1920.0f, 25.0f });
		SetTexture(&texture);
		Upload();
	}

protected:

	Peio::Win::Window& window;
	Peio::GUI::Texture texture = {};

	Peio::Int2 prev = {};
	//bool wait = false;

	void OnMouseMove(Peio::Win::MouseMoveEvent& event) override {
		//if (wait) {
		//	wait = false;
		//	return;
		//}
		if (event.wParam & (UINT64)Peio::Win::MouseKey::LEFT_MOUSE) {
			if (wasHovered) {
				Peio::Int2 delta = event.position - prev;
				RECT rect = window.GetRect();
				Peio::Int2 pos = { rect.left, rect.top };
				Peio::Int2 size = { rect.right - rect.left, rect.bottom - rect.top };
				window.Remap(pos + delta, size);
				//wait = true;
			}
		}
		else {
			prev = event.position;
		}
	}

};

struct CloseButton : public Peio::GUI::Button {

	CloseButton(Peio::Win::Window& window, Peio::Gfx::Graphics* graphics) : window(window) {
		texture.Init(graphics, Peio::Med::Images::Load("closeButton.png", AV_PIX_FMT_RGBA, { 50, 25 }), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();

		Init(graphics, { (float)graphics->GetSize().x() - 50.0f, 0.0f }, { 50.0f, 25.0f });
		SetTexture(&texture);
		Upload();
	}

protected:

	Peio::Win::Window& window;
	Peio::GUI::Texture texture = {};

	void OnMouseUp(Peio::Win::MouseButtonUpEvent& event) {
		window.Close();
		exit(0);
	}

};

struct MinButton : public Peio::GUI::Button {

	MinButton(Peio::Win::Window& window, Peio::Gfx::Graphics* graphics) : window(window) {
		texture.Init(graphics, Peio::Med::Images::Load("minButton.png", AV_PIX_FMT_RGBA, { 50, 25 }), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();

		Init(graphics, { (float)graphics->GetSize().x() - 100.0f, 0.0f }, { 50.0f, 25.0f });
		SetTexture(&texture);
		Upload();
	}

protected:

	Peio::Win::Window& window;
	Peio::GUI::Texture texture = {};

	void OnMouseUp(Peio::Win::MouseButtonUpEvent& event) {
		window.Minimize();
	}

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

		SetCursor(LoadCursor(nullptr, IDC_ARROW));

		Peio::Win::MouseListener mouseListener;
		Peio::Win::Input::AddListener(&mouseListener);

		Titlebar titleBar(window, &graphics);
		CloseButton closeButton(window, &graphics);
		MinButton minButton(window, &graphics);

		while (true) {
			window.HandleMessages();

			graphics.Clear({ 0.21f, 0.22f, 0.25f, 1.0f });
			
			titleBar.Draw();
			closeButton.Draw();
			minButton.Draw();
			//rect.Draw();
			//text.Draw();

			graphics.Render();
		}
	}
	catch (Peio::Win::Exception e) {
		std::cout << "GfxException: " << e.msg << " at " << e.file << " line " << e.line << " code " << e.winError << std::endl;
	}
	catch (Peio::Gfx::Exception e) {
		std::cout << "GfxException: " << e.msg << " at " << e.file << " line " << e.line << " ret " << e.ret << std::endl;
	}
	catch (Peio::Exception e) {
		std::cout << "Exception: " << e.msg << " at " << e.file << " line " << e.line << std::endl;
	}

	return 0;
}