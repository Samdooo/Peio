#include "..\..\App.h"

namespace Graphics::GUI {

	void Framerate::Init()
	{
		text.Init(&App::app.graphics.graphics, { 10.0f, 10.0f }, { 1000.0f, 1000.0f });
		text.SetFont(&App::app.graphics.gui.fonts.robotoLight);
		text.SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
		text.SetSpaceWidth(5.0f);
		text.SetString("Framerate: Calculating...");
		text.Upload();
	}

	void Framerate::Update()
	{
		frameCount++;
		if (clock.Elapsed().Seconds() >= updateFreq) {
			double fps = (double)frameCount / clock.Restart().Seconds();
			text.SetString("Framerate: " + std::to_string((size_t)fps));
			frameCount = 0;
		}
	}

	void Framerate::Render()
	{
		text.Draw();
	}

}