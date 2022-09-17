#pragma once

#include "Keybind.h"

namespace Input {

	struct BuildKeybinds {

		void Init();

		Keybind place = {};
		Keybind destroy = {};
		Keybind pick = {};
		Keybind fill = {};

	};

}