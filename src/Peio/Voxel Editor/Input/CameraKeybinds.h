#pragma once

#include "Keybind.h"

namespace Input {

	struct CameraKeybinds {

		void Init();

		Keybind moveForward = {};
		Keybind moveBackward = {};
		Keybind moveLeft = {};
		Keybind moveRight = {};
		Keybind moveUp = {};
		Keybind moveDown = {};

	};

}