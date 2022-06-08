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
#include <Peio/GUI/ValueAnimation.h>
#include <Peio/GUI/Animatable.h>
#include <Peio/GUI/PolyAnimation.h>

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

struct AnimatedButton : public virtual Peio::GUI::Button, public virtual Peio::GUI::Animatable {};

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

struct CloseButton : public AnimatedButton {

	CloseButton(Peio::Win::Window& window, Peio::Gfx::Graphics* graphics) : window(window) {
		texture.Init(graphics, Peio::Med::Images::Load("closeButton.png", AV_PIX_FMT_RGBA, { 50, 25 }), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();

		Init(graphics, { (float)graphics->GetSize().x() - 50.0f, 0.0f }, { 50.0f, 25.0f });
		SetColor({ 1.0f, 1.0f, 1.0f, 0.2f });
		SetTexture(&texture);
		Upload();

		//Peio::GUI::J_Animation<float, 4> enterAnimation = {};

		enterAnimation.duration = 0.4;
		enterAnimation.from = { 1.0f, 1.0f, 1.0f, 0.2f };
		enterAnimation.to = { 1.0f, 1.0f, 1.0f, 1.0f };
		enterAnimation.multiplier = 2.0;
		enterAnimation.UpdateValue = [this](Peio::Float4 color) {
			this->SetColor(color);
		};

		leaveAnimation.duration = 0.4;
		leaveAnimation.from = { 1.0f, 1.0f, 1.0f, 1.0f };
		leaveAnimation.to = { 1.0f, 1.0f, 1.0f, 0.2f };
		leaveAnimation.multiplier = 2.0;
		leaveAnimation.UpdateValue = [this](Peio::Float4 color) {
			this->SetColor(color);
		};

		AddAnimation("enter", &enterAnimation);
		AddAnimation("leave", &leaveAnimation);
	}

protected:

	Peio::Win::Window& window;
	Peio::GUI::Texture texture = {};
	
	Peio::GUI::J_Animation<float, 4> enterAnimation = {};
	Peio::GUI::J_Animation<float, 4> leaveAnimation = {};

	void OnMouseUp(Peio::Win::MouseButtonUpEvent& event) override {
		window.Close();
		exit(0);
	}

	void OnMouseEnter(Peio::Win::MouseMoveEvent& event) override {
		GetAnimation("leave")->Cancel();
		GetAnimation("enter")->Reset(1.0 - GetAnimation("leave")->GetProgress());
	}

	void OnMouseLeave(Peio::Win::MouseMoveEvent& event) override {
		GetAnimation("enter")->Cancel();
		GetAnimation("leave")->Reset(1.0 - GetAnimation("enter")->GetProgress());
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

struct TestButton : public AnimatedButton {

	TestButton(Peio::Win::Window& window, Peio::Gfx::Graphics* graphics) : window(window) {
		Init(graphics, { 400.0f, 300.0f }, { 100.0f, 100.0f });
		SetColor({ 0.0f, 0.5f, 1.0f, 1.0f });
		Upload();

		enterAnimation.duration = 1.0;
		enterAnimation.multiplier = 3.0;
		enterAnimation.from = { 100.0f, 100.0f };
		enterAnimation.to = { 500.0f, 100.0f };
		enterAnimation.UpdateValue = [this](Peio::Float2 size) {
			this->size = size;
		};

		leaveAnimation.duration = 1.0;
		leaveAnimation.multiplier = 3.0;
		leaveAnimation.from = { 500.0f, 100.0f };
		leaveAnimation.to = { 100.0f, 100.0f };
		leaveAnimation.UpdateValue = [this](Peio::Float2 size) {
			this->size = size;
		};

		AddAnimation("enter", &enterAnimation);
		AddAnimation("leave", &leaveAnimation);
	}

protected:

	void OnMouseEnter(Peio::Win::MouseMoveEvent& event) override {
		leaveAnimation.Cancel();
		enterAnimation.Reset(1.0 - leaveAnimation.Calc(leaveAnimation.GetProgress()));
	}

	void OnMouseLeave(Peio::Win::MouseMoveEvent& event) override {
		enterAnimation.Cancel();
		leaveAnimation.Reset(1.0 - enterAnimation.Calc(enterAnimation.GetProgress()));
	}

	Peio::Win::Window& window;

	Peio::GUI::J_Animation<float, 2> enterAnimation;
	Peio::GUI::J_Animation<float, 2> leaveAnimation;

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

		TestButton testButton(window, &graphics);

		Peio::Clock<double> clock;

		while (true) {
			window.HandleMessages();

			closeButton.Update();
			closeButton.Upload();

			testButton.Update();
			testButton.Upload();

			graphics.Clear({ 0.21f, 0.22f, 0.25f, 1.0f });
			
			titleBar.Draw();
			closeButton.Draw();
			minButton.Draw();

			testButton.Draw();

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