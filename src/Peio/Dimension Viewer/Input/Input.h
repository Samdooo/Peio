#pragma once

#include "State.h"
#include "Camera.h"

namespace Input {

	struct Input {

		State state = {};
		Camera camera = {};

		void Init();
		void Update();

	};

}