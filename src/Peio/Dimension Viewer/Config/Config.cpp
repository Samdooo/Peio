#include "Config.h"

#include <iostream>

namespace Config {

	void Config::Init()
	{
		scene.Init();
		objects.Init();
		std::cout << "Loaded config" << std::endl;
	}

}