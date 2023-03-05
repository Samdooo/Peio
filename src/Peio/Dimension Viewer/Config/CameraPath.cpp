#include "..\App.h"

#include "..\Video\Path.h"
#include <iostream>

namespace Config {

	bool CameraPath::Init()
	{
		const UINT numDims = App::app.scene.numDims;

		YAML::Node points = YAML::LoadFile(App::app.config.configPath + "cameraPath.yaml")["path"];
		if (!points) {
			throw PEIO_EXCEPTION("Failed to load scene.yaml");
		}
		std::vector<::Video::Path::Point> targets(points.size());
		std::vector<UINT> durations(points.size() - 1);
		for (size_t i = 0; i < points.size(); i++) {
			targets[i].position.resize(numDims);
			targets[i].rotation.resize(numDims - 1);
			for (UINT j = 0; j < numDims; j++)
				targets[i].position[j] = points[i]["position"][j].as<float>();
			for (UINT j = 0; j < numDims - 1; j++)
				targets[i].rotation[j] = points[i]["rotation"][j].as<float>();
			if (i < points.size() - 1)
				durations[i] = (UINT)points[i]["duration"].as<int>();
		}

		for (size_t i = 0; i < targets.size() - 1; i++) {
			for (UINT j = 0; j < durations[i]; j++) {
				float progress = (float)j / (float)durations[i];
				::Video::Path::Point point = {};
				point.position = targets[i].position * (1.0f - progress) + targets[i + 1].position * progress;
				point.rotation = targets[i].rotation * (1.0f - progress) + targets[i + 1].rotation * progress;
				App::app.videoGraphics.path.points.push_back(point);
			}
		}
		App::app.videoGraphics.path.points.push_back(targets.back());

		std::cout << "Loaded camera path (" << App::app.videoGraphics.path.points.size() << " frames)" << std::endl;
		return true;
	}

}