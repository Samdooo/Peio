#include "..\..\App.h"

namespace Graphics::GUI {

	void Fonts::Init()
	{
		robotoLight.Init(&App::app.graphics.graphics, "Resources/Fonts/Roboto-Light.ttf", 16);
		robotoLight.LoadLetters();
		robotoLight.LoadTextures();
	}

}