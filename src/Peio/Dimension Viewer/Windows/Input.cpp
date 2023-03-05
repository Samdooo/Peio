#include "..\App.h"

#include "..\..\Windows\RawMouseListener.h"
#include "..\..\Windows\KeyboardListener.h"

#include <iostream>

namespace Windows {

	using namespace Peio;

	void Input::Init() 
	{
		if (!App::app.init) {
			Win::RawMouseListener::Register(App::app.winGraphics.window.GetHWND());
			Win::Input::listeners.Add(Win::keyboardListener);
			Win::Input::listeners.Add(Win::rawMouseListener);
		}

		state.Init();
		camera.Init();

		std::cout << "Initialized input" << std::endl;
	}

	void Input::Update()
	{
		camera.Update();
	}

}