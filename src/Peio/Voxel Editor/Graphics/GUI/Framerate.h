#pragma once

#include "..\..\..\GUI\Text.h"

namespace Graphics::GUI {

	struct Framerate {

		void Init();
		void Update();
		void Render();

		Peio::GUI::Text text = {};
		Peio::Clock<double> clock = {};
		size_t frameCount = 0;
		double elapsed = 0.0;
		double updateFreq = 1.0;

	};

}