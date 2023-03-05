#include "..\App.h"

#include <iostream>

namespace Config {

	void Scene::Init()
	{
		YAML::Node scene = YAML::LoadFile(App::app.config.configPath + "scene.yaml");
		if (!scene) {
			throw PEIO_EXCEPTION("Failed to load scene.yaml");
		}
		std::string type;
		try { type = scene["type"].as<std::string>(); }
		catch (...) { throw PEIO_EXCEPTION("Failed to get scene type."); }

		UINT numDims;
		try {
			numDims = scene["numDims"].as<int>();
			App::app.scene.numDims = numDims;
			App::app.scene.info.screenSize = { (UINT)scene["screenSize"][0].as<int>(), (UINT)scene["screenSize"][1].as<int>() };
		} catch (...) { throw PEIO_EXCEPTION("Failed to get scene information."); }

		YAML::Node camera;
		try {
			camera = scene["camera"];
			App::app.scene.camera.aspectRatio = (float)App::app.scene.info.screenSize.y() / (float)App::app.scene.info.screenSize.x();
			App::app.scene.camera.fov = camera["fov"].as<float>();
		}
		catch (...) { throw PEIO_EXCEPTION("Failed to get camera information."); }

		try {
			YAML::Node renderer = scene["renderer"];

			YAML::Node rays = renderer["rays"];
			App::app.scene.maxBounces = rays["maxBounces"].as<int>();
			App::app.scene.rays.perBounce.resize(App::app.scene.maxBounces - 1);
			for (UINT i = 0; i < App::app.scene.rays.perBounce.size(); i++)
				App::app.scene.rays.perBounce[i] = rays["raysPerBounce"][i].as<int>();

			YAML::Node denoiser = renderer["denoiser"];
			App::app.scene.info.denoiser.radius = denoiser["radius"].as<int>();
			App::app.scene.info.denoiser.maxDist = denoiser["maxDist"].as<float>();
			App::app.scene.info.denoiser.maxAngle = denoiser["maxAngle"].as<float>();
		} catch (...) { throw PEIO_EXCEPTION("Failed to get renderer information."); }

		if (type == "live") {
			App::app.isVideo = false;
			try {
				YAML::Node liveCamera = camera["live"];
				if (App::app.scene.camera.position.size() != numDims) {
					App::app.scene.camera.position.resize(numDims);
					for (UINT i = 0; i < numDims; i++)
						App::app.scene.camera.position[i] = liveCamera["position"][i].as<float>();
				}
				if (App::app.scene.camera.rotation.size() != numDims - 1) {
					App::app.scene.camera.rotation.resize(numDims - 1);
					for (UINT i = 0; i < numDims - 1; i++)
						App::app.scene.camera.rotation[i] = liveCamera["rotation"][i].as<float>();
				}
				App::app.winGraphics.input.camera.speed = liveCamera["speed"].as<float>();
				App::app.winGraphics.input.camera.acceleration = liveCamera["acceleration"].as<float>();
			} catch (...) { throw PEIO_EXCEPTION("Failed to get live camera information."); }
			App::app.config.controls.Init();
		}
		else {
			App::app.isVideo = true;
			try {
				YAML::Node videoCamera = camera["video"];
				App::app.videoGraphics.framerate = videoCamera["framerate"].as<int>();
				App::app.videoGraphics.bitrate = videoCamera["bitrate"].as<int>();
				App::app.videoGraphics.outPath = videoCamera["outpath"].as<std::string>();
			} catch (...) { throw PEIO_EXCEPTION("Failed to get video camera information."); }
			App::app.config.cameraPath.Init();
		}
		std::cout << "Loaded scene" << std::endl;
	}

}