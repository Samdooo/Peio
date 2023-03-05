#include "..\App.h"

#include <iostream>

namespace Windows {

	void WinGraphics::Init()
	{
		if (App::app.init) {
			if (graphics.GetSize() != (Long2)App::app.scene.info.screenSize) {
				//HDWP hdwp = BeginDeferWindowPos(1);
				//DeferWindowPos(hdwp, window.GetHWND(), NULL, position.x(), position.y(),
				//	App::app.scene.info.screenSize.x(), App::app.scene.info.screenSize.y(), SWP_NOZORDER);
				//EndDeferWindowPos(hdwp);
				////graphics.Resize(App::app.scene.info.screenSize, 3);
				//graphics.~WinGraphics();
				//graphics.Init(window.GetHWND(), App::app.scene.info.screenSize, 3, false);
				//std::cout << "Resized window graphics to " << App::app.scene.info.screenSize.ToString() << std::endl;
				//DestroyWindow(window.GetHWND());
				//window.CreateWindow("Dimension Viewer", WS_POPUP | WS_VISIBLE, 0, { 0, 0 }, App::app.scene.info.screenSize);
				throw PEIO_EXCEPTION("Screen resizing not allowed.");
			}
		}
		else {
			window.CreateClass("Dimension Viewer Class", 0);
			window.RegisterClass();
			window.CreateWindow("Dimension Viewer", WS_POPUP | WS_VISIBLE, 0, { 0, 0 }, App::app.scene.info.screenSize);
			graphics.Init(window.GetHWND(), App::app.scene.info.screenSize, 3, false);
			std::cout << "Initialized window graphics" << std::endl;
		}
		input.Init();
	}

	void WinGraphics::Update()
	{
		window.HandleMessages();
		input.Update();
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