#include "App.h"

App App::app = {};

void App::Init()
{
	graphics.Init();
	scene.Init();
	input.Init();
	graphics.gui.Init();
	App::app.input.state.SetState(State::PAUSED);
}

bool App::Update()
{
	input.Update();
	scene.Update();
	graphics.Update();
	return true;
}
