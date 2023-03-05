#pragma once

#include "..\..\Windows\Window.h"
#include "..\..\Graphics\MediaGraphics.h"
#include "..\..\Media\Encoder.h"
#include "Path.h"
#include <fstream>
#include <string>

namespace Video {

	using namespace Peio;

	struct VideoGraphics {

		Gfx::MediaGraphics graphics = {};
		Med::Encoder encoder = {};
		std::ofstream ofile = {};
		std::string outPath = {};
		int framerate = 0;
		int bitrate = 0;
		
		Path path = {};

		void Init();
		bool Update();
		void Render();
		void Cleanup();

	};

}