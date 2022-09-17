#pragma once

#include "Hotkey.h"

namespace Input {

	struct Hotkeys : protected Peio::EventHandler<Peio::Win::RawKeyDownEvent, Peio::Win::RawKeyUpEvent> {

		void Init();

		Peio::Win::RawKeyboardListener listener = {};
		std::unordered_map<USHORT, Hotkey> hotkeys = {};

	protected:

		bool Handle(Peio::Win::RawKeyDownEvent* event) override;
		bool Handle(Peio::Win::RawKeyUpEvent* event) override;

	};

}