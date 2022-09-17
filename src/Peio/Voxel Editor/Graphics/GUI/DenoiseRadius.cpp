#include "..\..\App.h"

namespace Graphics::GUI {

	void DenoiseRadius::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 2.0f) }, { 1000.0f, 1000.0f });
		text.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		text.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		text.Upload();
		text.SetSpaceWidth(5.0f);

		Peio::GUI::Button::Init(&App::app.graphics.graphics, { text.position.x() - 2.0f, text.position.y() }, {0.0f, 20.0f});
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();

		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());

		Update();
	}

	void DenoiseRadius::Update()
	{
		text.SetString("Denoise radius: " + std::to_string(App::app.scene.info.scene.denoiseRadius));
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void DenoiseRadius::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void DenoiseRadius::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void DenoiseRadius::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void DenoiseRadius::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		if (event->delta > 0) {
			if (App::app.scene.info.scene.denoiseRadius < maxRadius)
				App::app.scene.info.scene.denoiseRadius++;
		}
		else {
			if (App::app.scene.info.scene.denoiseRadius > 0)
				App::app.scene.info.scene.denoiseRadius--;
		}
		Update();
	}

}