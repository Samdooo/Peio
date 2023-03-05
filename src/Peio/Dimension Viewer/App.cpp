#include "App.h"

#include <iostream>

App App::app = {};

void App::Init() 
{
	config.Init();
	if (!init)
		Peio::Gfx::Init();
	scene.Init();
	if (isVideo)
		videoGraphics.Init();
	else
		winGraphics.Init();
	rayRenderer.Init();
	denoiser.Init();
	if (!init) {
		
	}
	init = true;
}

bool App::Update()
{
	if (isVideo) {
		if (!videoGraphics.Update())
			return false;
		videoGraphics.Render();
	}
	else {
		winGraphics.Update();
		winGraphics.Render();
	}
	return true;
}

void App::Cleanup()
{
	if (isVideo)
		videoGraphics.Cleanup();
}
