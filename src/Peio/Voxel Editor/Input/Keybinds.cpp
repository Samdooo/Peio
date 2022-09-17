#include "..\App.h"

namespace Input {

	void Keybinds::Init()
	{
		camera.Init();
		build.Init();
		state.Init();

		Peio::Win::RawKeyboardListener::Register(App::app.graphics.window.window.GetHWND());
		Peio::Win::RawMouseListener::Register(App::app.graphics.window.window.GetHWND());

		Peio::Win::Input::AddListener(&rawKeyboard);
		Peio::Win::Input::AddListener(&rawMouse);

		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());
	}

	void Keybinds::AddKeybind(Keybind* keybind)
	{
		map.insert({ keybind->key, keybind });
	}

	bool Keybinds::Handle(Peio::Win::RawKeyDownEvent* event)
	{
		Key key = { Key::Type::KEYBOARD, event->key };
		if (map.contains(key)) {
			Keybind* keybind = map.at(key);
			if (!keybind->pressed) {
				keybind->pressed = true;
				if (keybind->OnPress)
					keybind->OnPress();
			}
		}
		return false;
	}

	bool Keybinds::Handle(Peio::Win::RawKeyUpEvent* event)
	{
		Key key = { Key::Type::KEYBOARD, event->key };
		if (map.contains(key)) {
			Keybind* keybind = map.at(key);
			if (keybind->pressed) {
				keybind->pressed = false;
				if (keybind->OnRelease)
					keybind->OnRelease();
			}
		}
		return false;
	}

	bool Keybinds::Handle(Peio::Win::RawMouseButtonDownEvent* event)
	{
		Key key = { Key::Type::MOUSE, (int)event->button };
		if (map.contains(key)) {
			Keybind* keybind = map.at(key);
			if (!keybind->pressed) {
				keybind->pressed = true;
				if (keybind->OnPress)
					keybind->OnPress();
			}
		}
		return false;
	}

	bool Keybinds::Handle(Peio::Win::RawMouseButtonUpEvent* event)
	{
		Key key = { Key::Type::MOUSE, (int)event->button };
		if (map.contains(key)) {
			Keybind* keybind = map.at(key);
			if (keybind->pressed) {
				keybind->pressed = false;
				if (keybind->OnRelease)
					keybind->OnRelease();
			}
		}
		return false;
	}

}