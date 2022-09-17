#include "..\App.h"

namespace Input {

	void Input::Init()
	{
		keybinds.Init();
		camera.Init();
		build.Init();
		state.Init();
		file.Init();
	}

	void Input::Update()
	{
		camera.Update();
		build.Update();
	}

}