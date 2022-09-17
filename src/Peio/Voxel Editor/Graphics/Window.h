#pragma once

#include "..\..\Windows\Window.h"

namespace Graphics {

	struct Window {

		void Init();
		void Update();

		const Peio::Uint2 windowSize = { 1280, 720 };
		Peio::Win::Window window = {};

	};

}