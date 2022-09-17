#pragma once

#include "Action.h"
#include "..\..\Windows\RawKeyboardListener.h"

namespace Input {

	struct Hotkey {

		USHORT key = {};
		Action actionPressed = {};
		Action actionReleased = {};

	};

}