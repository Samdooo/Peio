#include "..\App.h"

#include <iostream>

namespace Config {

	void Controls::Init()
	{
		YAML::Node controls = YAML::LoadFile(App::app.config.configPath + "controls.yaml");
		if (!controls) {
			throw PEIO_EXCEPTION("Failed to load controls.yaml");
		}

		try {
			App::app.winGraphics.input.state.toggleKey = controls["toggleState"].as<int>();

			App::app.winGraphics.input.camera.moveDimensionKeys = {
				controls["moveDimension"][0].as<int>(), controls["moveDimension"][1].as<int>() };

			YAML::Node movement = controls["movement"];
			App::app.winGraphics.input.camera.movementKeys.resize(movement.size());
			for (size_t i = 0; i < movement.size(); i++) {
				App::app.winGraphics.input.camera.movementKeys[i] = {
					movement[i][0].as<int>(), movement[i][1].as<int>()
				};
			}
			App::app.winGraphics.input.camera.mouseSensitivity = controls["mouseSensitivity"].as<float>();
		} catch (...) { throw PEIO_EXCEPTION("Failed to load controls."); }
		std::cout << "Loaded controls" << std::endl;
	}

}