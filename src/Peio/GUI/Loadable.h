#pragma once

#include "GUIHeader.h"
#include <fstream>

namespace Peio::GUI {

	template <typename T>
	struct Loadable {

		static void Load(T*, std::ifstream&) {}
		static void Save(const T*, std::ofstream&) {}

	};

}