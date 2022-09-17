#pragma once

#include "Action.h"
#include "..\Scene\Camera.h"

namespace Input {

	struct CameraActions {

		Action moveForward = {};
		Action moveBackward = {};
		Action moveLeft = {};
		Action moveRight = {};
		Action moveUp = {};
		Action moveDown = {};

		void Init();

	};

}