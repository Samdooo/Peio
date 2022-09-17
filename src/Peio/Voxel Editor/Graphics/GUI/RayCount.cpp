#include "..\..\App.h"

namespace Graphics::GUI {

	void RayCount::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 1.0f) }, { 1000.0f, 1000.0f });
		text.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		text.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		text.Upload();
		text.SetSpaceWidth(5.0f);

		Peio::GUI::Button::Init(&App::app.graphics.graphics, { text.position.x() - 2.0f, text.position.y() }, { 0.0f, 20.0f });
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();

		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());

		Update();
	}

	void RayCount::Update()
	{
		text.SetString("Light bounces: " + std::to_string(App::app.scene.info.scene.numRays));
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void RayCount::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void RayCount::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void RayCount::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void RayCount::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		if (event->delta > 0) {
			if (App::app.scene.info.scene.numRays < maxRayCount)
				App::app.scene.info.scene.numRays++;
		}
		else {
			if (App::app.scene.info.scene.numRays > 1)
				App::app.scene.info.scene.numRays--;
		}
		Update();
	}

}