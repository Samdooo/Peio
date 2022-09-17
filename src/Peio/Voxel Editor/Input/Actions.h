#pragma once

#include "BuildActions.h"
#include "CameraActions.h"

namespace Input {

	struct Actions {

		void Init();

		BuildActions buildActions = {};
		CameraActions cameraActions = {};

		std::vector<Action*> actions = {};

	};

}