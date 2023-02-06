#include "App.h"

App App::app = {};

void App::Init() 
{
	Peio::Gfx::Init();
	scene.Init();
	winGraphics.Init();
	rayRenderer.Init();
	denoiser.Init();
}

bool App::Update()
{
	winGraphics.Update();
	winGraphics.Render();
	return true;
}