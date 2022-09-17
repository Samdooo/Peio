#pragma once

#include "Info.h"
#include "World.h"
#include "Camera.h"
#include "RayField.h"

namespace Scene {

	struct Scene {

		void Init();
		void Update();

		Info info = {};
		World world = {};
		Camera camera = {};
		RayField rayField = {};

	};

}