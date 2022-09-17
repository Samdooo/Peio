#include "..\App.h"

namespace Input {

	void CameraKeybinds::Init()
	{
		moveForward.key = { Key::Type::KEYBOARD, 'W' };
		moveBackward.key = { Key::Type::KEYBOARD, 'S' };
		moveLeft.key = { Key::Type::KEYBOARD, 'A' };
		moveRight.key = { Key::Type::KEYBOARD, 'D' };
		moveUp.key = { Key::Type::KEYBOARD, VK_SPACE };
		moveDown.key = { Key::Type::KEYBOARD, VK_SHIFT };

		App::app.input.keybinds.AddKeybind(&moveForward);
		App::app.input.keybinds.AddKeybind(&moveBackward);
		App::app.input.keybinds.AddKeybind(&moveLeft);
		App::app.input.keybinds.AddKeybind(&moveRight);
		App::app.input.keybinds.AddKeybind(&moveUp);
		App::app.input.keybinds.AddKeybind(&moveDown);
	}

}