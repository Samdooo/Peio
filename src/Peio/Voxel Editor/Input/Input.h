#pragma once

#include "Keybinds.h"
#include "CameraInput.h"
#include "BuildInput.h"
#include "StateInput.h"
#include "FileInput.h"

namespace Input {

	struct Input {

		void Init();
		void Update();

		Keybinds keybinds = {};

		CameraInput camera = {};
		BuildInput build = {};
		StateInput state = {};
		FileInput file = {};

	};

}