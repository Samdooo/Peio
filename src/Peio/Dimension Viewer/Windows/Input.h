#pragma once

#include "State.h"
#include "Camera.h"

namespace Windows {

	struct Input {

		State state = {};
		Camera camera = {};

		void Init();
		void Update();

	};

}