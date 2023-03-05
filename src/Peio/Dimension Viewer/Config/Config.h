#pragma once

#define YAML_CPP_STATIC_DEFINE

#include "Scene.h"
#include "Controls.h"
#include "CameraPath.h"
#include "Objects.h"

#pragma comment(lib, "yaml-cpp/yaml-cpp.lib")

namespace Config {

	struct Config {

		std::string configPath = "Config/";
		Scene scene = {};
		Controls controls = {};
		CameraPath cameraPath = {};
		Objects objects = {};

		void Init();

	};

}