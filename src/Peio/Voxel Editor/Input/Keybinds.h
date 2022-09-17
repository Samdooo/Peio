#pragma once

#include <unordered_map>
#include "..\..\Windows\RawKeyboardListener.h"
#include "..\..\Windows\RawMouseListener.h"

#include "CameraKeybinds.h"
#include "BuildKeybinds.h"
#include "StateKeybinds.h"

namespace Input {

	struct Keybinds : protected Peio::EventHandler<Peio::Win::RawKeyDownEvent, Peio::Win::RawKeyUpEvent,
		Peio::Win::RawMouseButtonDownEvent, Peio::Win::RawMouseButtonUpEvent>
	{

		void Init();
		void AddKeybind(Keybind* keybind);

		CameraKeybinds camera = {};
		BuildKeybinds build = {};
		StateKeybinds state = {};

	protected:

		std::unordered_map<Key, Keybind*, Key::Hash> map = {};
		Peio::Win::RawKeyboardListener rawKeyboard = {};
		Peio::Win::RawMouseListener rawMouse = {};

		bool Handle(Peio::Win::RawKeyDownEvent* event) override;
		bool Handle(Peio::Win::RawKeyUpEvent* event) override;
		bool Handle(Peio::Win::RawMouseButtonDownEvent* event) override;
		bool Handle(Peio::Win::RawMouseButtonUpEvent* event) override;

	};

}