#include "..\App.h"

namespace Scene {

	void Camera::Init()
	{
		Peio::Uint2 windowSize = App::app.graphics.window.windowSize;
		smoothCam.camera = {
			{ (float)(1 << 13) + 0.01f, 300.0f, (float)(1 << 13) + 0.01f},
			{},
			1.57f,
			(float)windowSize.y() / (float)windowSize.x()
		};
		smoothCam.acceleration = 30.0f;
		smoothCam.targetVelocity = 50.0f;
		buffer.SetBuffer(&smoothCam.camera, 1);
		srv.Init(buffer.GetBufferSize(), buffer.GetNumElements());
		Update();
		Upload();
	}

	void Camera::Upload()
	{
		srv.Upload(buffer.GetResourceData(), App::app.graphics.GetCommandList());
		lastUpload = smoothCam.camera;
	}

	void Camera::Update()
	{
		smoothCam.Update();
		if (smoothCam.camera != lastUpload) {
			App::app.scene.info.scene.newRays = true;
			Upload();
		}
	}

}