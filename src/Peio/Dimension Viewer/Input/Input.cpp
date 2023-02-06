#include "..\App.h"

#include "..\..\Windows\RawMouseListener.h"
#include "..\..\Windows\KeyboardListener.h"

namespace Input {

	using namespace Peio;

	void Input::Init() 
	{
		Win::RawMouseListener::Register(App::app.winGraphics.window.GetHWND());
		Win::Input::listeners.Add(Win::keyboardListener);
		Win::Input::listeners.Add(Win::rawMouseListener);

		state.Init();
		camera.Init();
	}

	void Input::Update()
	{
		camera.Update();
	}

}