#include "..\..\App.h"

namespace Graphics::GUI {

	void GUI::Init()
	{
		Peio::Win::Input::AddListener(&mouseListener);
		Peio::Win::Input::AddListener(&textListener);
		Peio::GUI::Rectangle::Init();

		fonts.Init();
		framerate.Init();
		rayCount.Init();
		denoiseRadius.Init();
		speed.Init();
		material.Init();
		place.Init();
		destroy.Init();
		fill.Init();
	}

	void GUI::Update()
	{
		framerate.Update();
	}

	void GUI::Render()
	{
		framerate.Render();
		rayCount.Render();
		denoiseRadius.Render();
		speed.Render();
		material.Render();
		place.Render();
		destroy.Render();
		fill.Render();
	}

}