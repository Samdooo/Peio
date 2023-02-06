#pragma once

#include "..\..\Windows\Window.h"
#include "..\..\Graphics\WinGraphics.h"

namespace Graphics {

	using namespace Peio;

	struct WinGraphics {

		Win::Window window = {};
		Gfx::WinGraphics graphics = {};

		void Init();
		void Update();
		void Render();

	};

}