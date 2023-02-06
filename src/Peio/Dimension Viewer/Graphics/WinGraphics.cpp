#include "..\App.h"

namespace Graphics {

	void WinGraphics::Init()
	{
		window.CreateClass("Dimension Viewer Class", 0);
		window.RegisterClass();
		window.CreateWindow("Dimension Viewer", WS_POPUP | WS_VISIBLE, 0, { 0, 0 }, App::app.scene.info.screenSize);
		
		App::app.input.Init();

		graphics.Init(window.GetHWND(), App::app.scene.info.screenSize, 3, false);
	}

	void WinGraphics::Update()
	{
		window.HandleMessages();
		App::app.input.Update();
		App::app.scene.Update(graphics.GetCommandList());
		App::app.rayRenderer.Update(graphics.GetCommandList());
		App::app.denoiser.Update(graphics.GetCommandList());
	}

	void WinGraphics::Render()
	{
		graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
		App::app.rayRenderer.Render(graphics.GetCommandList());
		App::app.denoiser.Render(graphics.GetCommandList());
		graphics.Render();
	}

}