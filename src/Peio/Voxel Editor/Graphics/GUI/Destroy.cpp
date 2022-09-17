#include "..\..\App.h"

namespace Graphics::GUI {

	void Destroy::Radius::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 8.0f) }, { 1000.0f, 1000.0f });
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

	void Destroy::Radius::Update()
	{
		text.SetString("Destroy radius: " + std::to_string(App::app.input.build.destroy.radius));
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void Destroy::Radius::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void Destroy::Radius::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void Destroy::Radius::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void Destroy::Radius::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		if (event->delta > 0) {
			App::app.input.build.destroy.radius++;
		}
		else {
			if (App::app.input.build.destroy.radius > 1)
				App::app.input.build.destroy.radius--;
		}
		Update();
	}

	void Destroy::Shape::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f + (20.0f * 9.0f) }, { 1000.0f, 1000.0f });
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

	void Destroy::Shape::Update()
	{
		std::string shapeName = "";
		switch (App::app.input.build.destroy.shape) {
		case Input::BuildInput::Shape::CUBE:
			shapeName = "Cube";
			break;
		case Input::BuildInput::Shape::SPHERE:
			shapeName = "Sphere";
			break;
		};
		text.SetString("Destroy shape: " + shapeName);
		Peio::GUI::Button::size.x() = text.GetWidth() + 4.0f;
	}

	void Destroy::Shape::Render()
	{
		Peio::GUI::Button::Draw();
		text.Draw();
	}

	void Destroy::Shape::OnMouseEnter(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
		Peio::GUI::Button::Upload();
	}

	void Destroy::Shape::OnMouseLeave(Peio::Win::MouseMoveEvent*)
	{
		Peio::GUI::Button::SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Peio::GUI::Button::Upload();
	}

	void Destroy::Shape::OnMouseWheel(Peio::Win::MouseWheelEvent* event)
	{
		const int numShapes = 2;
		int shape = (int)App::app.input.build.destroy.shape;
		if (event->delta > 0) {
			shape++;
			shape %= numShapes;
		}
		else {
			shape--;
			if (shape == -1)
				shape = numShapes - 1;
		}
		App::app.input.build.destroy.shape = (Input::BuildInput::Shape)shape;
		Update();
	}


	void Destroy::Init()
	{
		radius.Init();
		shape.Init();
	}

	void Destroy::Render()
	{
		radius.Render();
		shape.Render();
	}

}