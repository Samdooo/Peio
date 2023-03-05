#include "..\App.h"

#include <iostream>

namespace Config {

	void Objects::Init()
	{
		const UINT numDims = App::app.scene.numDims;

		App::app.scene.objects.hyperRectangles.rects.clear();
		App::app.scene.objects.hyperSpheres.spheres.clear();

		YAML::Node objects = YAML::LoadFile(App::app.config.configPath + "objects.yaml")["objects"];
		if (!objects) {
			throw PEIO_EXCEPTION("Failed to load scene.yaml");
		}
		for (YAML::Node obj : objects) {
			::Scene::Material material = {};
			try {
				for (UINT i = 0; i < 3; i++) {
					material.emission[i] = obj["material"]["emission"][i].as<float>();
					material.reflection[i] = obj["material"]["reflection"][i].as<float>();
				}
				material.specular = obj["material"]["specular"].as<float>();
			} catch (...) { throw PEIO_EXCEPTION("Failed to load material."); }

			std::string type = obj["type"].as<std::string>();
			if (type == "hyperRectangle") {
				try {
					::Scene::HyperRectangle rect = {};
					rect.low.resize(numDims);
					rect.high.resize(numDims);
					rect.rotation.resize(numDims - 1);
					for (UINT i = 0; i < numDims; i++) {
						rect.low[i] = obj["low"][i].as<float>();
						rect.high[i] = obj["high"][i].as<float>();
						if (i < numDims - 1)
							rect.rotation[i] = obj["rotation"][i].as<float>();
					}
					rect.material = material;
					App::app.scene.objects.hyperRectangles.rects.push_back(rect);
				} catch (...) { throw PEIO_EXCEPTION("Failed to load hyperrectangle."); }
			}
			else if (type == "hyperSphere") {
				try {
					::Scene::HyperSphere sphere = {};
					sphere.center.resize(numDims);
					for (UINT i = 0; i < numDims; i++)
						sphere.center[i] = obj["center"][i].as<float>();
					sphere.radius = obj["radius"].as<float>();
					sphere.material = material;
					App::app.scene.objects.hyperSpheres.spheres.push_back(sphere);
				} catch (...) { throw PEIO_EXCEPTION("Failed to load hypersphere."); }
			}
			else {
				throw PEIO_EXCEPTION("Invalid object type in config.");
			}
		}
		std::cout << "Loaded objects" << std::endl;
	}

}