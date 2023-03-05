#include "..\App.h"

namespace Video {

	void Path::Init()
	{

	}

	bool Path::Update()
	{
		int frameIndex = App::app.videoGraphics.encoder.GetFrameIndex();
		if (frameIndex == (int)points.size())
			return false;
		App::app.scene.camera.position = points[frameIndex].position;
		App::app.scene.camera.rotation = points[frameIndex].rotation;
		return true;
	}

}