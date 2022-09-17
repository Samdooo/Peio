#include "..\App.h"

namespace Input {

	void BuildKeybinds::Init()
	{
		place.key = { Key::Type::MOUSE, (int)Peio::Win::MouseButton::RIGHT };
		destroy.key = { Key::Type::MOUSE, (int)Peio::Win::MouseButton::LEFT };
		pick.key = { Key::Type::MOUSE, (int)Peio::Win::MouseButton::MIDDLE };
		fill.key = { Key::Type::KEYBOARD, 'F' };

		App::app.input.keybinds.AddKeybind(&place);
		App::app.input.keybinds.AddKeybind(&destroy);
		App::app.input.keybinds.AddKeybind(&pick);
		App::app.input.keybinds.AddKeybind(&fill);
	}

}