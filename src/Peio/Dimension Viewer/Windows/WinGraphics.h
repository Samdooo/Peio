#pragma once

#include "..\..\Windows\Window.h"
#include "..\..\Graphics\WinGraphics.h"
#include "Input.h"

namespace Windows {

	using namespace Peio;

	struct WinGraphics {

		Uint2 position = {};

		Win::Window window = {};
		Gfx::WinGraphics graphics = {};
		Input input = {};

		void Init();
		void Update();
		void Render();

	};

}