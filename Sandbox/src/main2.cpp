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
#include <Peio/TypeMap.h>
#include <Peio/GUI/LoadMap.h>
#include <Peio/GUI/LoadAnimation.h>
#include <Peio/GUI/LoadValueAnimation.h>

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

struct CloseButton : public Peio::GUI::Button {

	CloseButton(Peio::Gfx::Graphics* graphics, Peio::Win::Window& window) : window(window) {
		Peio::GUI::Button::Init(graphics, { (float)graphics->GetSize().x() - 50, 0 }, { 50, 25 });
		texture.Init(graphics, Peio::Med::Images::Load("closeButton.png", AV_PIX_FMT_RGBA), DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.Upload();
		SetTexture(&texture);
		SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Upload();

		jFnc.multiplier = 2.0;

		enterAnimation.duration = 0.3;
		enterAnimation.from = { 1.0f, 1.0f, 1.0f, 0.3f };
		enterAnimation.to = { 1.0f, 1.0f, 1.0f, 1.0f };
		enterAnimation.numValues = 4;
		enterAnimation.function = &jFnc;
		enterAnimation.UpdateValues = [this](const float* values) {
			this->SetColor(*(Peio::Float4*)values);
			this->Upload();
		};
	}

	void OnMouseUp(Peio::Win::MouseButtonUpEvent* event) override {
		if (event->button == Peio::Win::MouseButton::LEFT) {
			exit(0);
		}
	}

	void OnMouseEnter(Peio::Win::MouseMoveEvent*) override {
		enterAnimation.Reset();
	}

	void OnMouseLeave(Peio::Win::MouseMoveEvent*) override {
		enterAnimation.Reset(true, enterAnimation.GetProgress());
	}

	void Update() {
		enterAnimation.Update();
	}

protected:

	Peio::Win::Window& window;
	Peio::GUI::Texture texture;

	Peio::GUI::J_Function jFnc = {};
	Peio::GUI::ValueAnimation<float> enterAnimation = {};

};

struct Base {
	virtual ~Base() {}
};
struct Child1 : public Base {};
struct Child2 : public Child1 {};

struct TestHandler : public virtual Peio::EventHandler<Child1, Child2> {

	bool Handle(Child1* c) override { std::cout << "Child1" << std::endl; }
	bool Handle(Child2* c) override { std::cout << "Child2" << std::endl; }

};

int main() {
	//
	//TestHandler handler;
	//
	//Peio::BaseHandlerSet<Base> set;
	//set.Insert(&handler, handler.GetBaseHandler<Base>());
	//
	//set.Handle(new Child1);
	//set.Handle(new Child2);

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

		CloseButton closeButton(&graphics, window);

		Peio::Win::Input::eventHandlers.Insert(&closeButton, closeButton.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::Clock<double> clock;

		while (true) {
			window.HandleMessages();

			closeButton.Update();

			graphics.Clear({ 0.21f, 0.22f, 0.25f, 1.0f });
			
			closeButton.Draw();

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