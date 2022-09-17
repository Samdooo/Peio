#include "..\..\App.h"

namespace Graphics::GUI {

	void Place::Radius::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 6.0f) }, { 1000.0f, 1000.0f });
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

	void Place::Radius::Update()
	{
		text.SetString("Build radius: " + std::to_string(App::app.input.build.place.radius));
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void Place::Radius::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void Place::Radius::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void Place::Radius::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void Place::Radius::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		if (event->delta > 0) {
			App::app.input.build.place.radius++;
		}
		else {
			if (App::app.input.build.place.radius > 1)
				App::app.input.build.place.radius--;
		}
		Update();
	}

	void Place::Shape::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 7.0f) }, { 1000.0f, 1000.0f });
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

	void Place::Shape::Update()
	{
		std::string shapeName = "";
		switch (App::app.input.build.place.shape) {
		case Input::BuildInput::Shape::CUBE:
			shapeName = "Cube";
			break;
		case Input::BuildInput::Shape::SPHERE:
			shapeName = "Sphere";
			break;
		};
		text.SetString("Build shape: " + shapeName);
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void Place::Shape::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void Place::Shape::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void Place::Shape::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void Place::Shape::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		const int numShapes = 2;
		int shape = (int)App::app.input.build.place.shape;
		if (event->delta > 0) {
			shape++;
			shape %= numShapes;
		}
		else {
			shape--;
			if (shape == -1)
				shape = numShapes - 1;
		}
		App::app.input.build.place.shape = (Input::BuildInput::Shape)shape;
		Update();
	}


	void Place::Init()
	{
		radius.Init();
		shape.Init();
	}

	void Place::Render()
	{
		radius.Render();
		shape.Render();
	}

}