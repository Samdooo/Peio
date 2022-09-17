#include "..\App.h"

namespace Input {

	void StateKeybinds::Init()
	{
		pause.key = { Key::Type::KEYBOARD, VK_ESCAPE };
		
		App::app.input.keybinds.AddKeybind(&pause);
	}

}