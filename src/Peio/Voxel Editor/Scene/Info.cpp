#include "..\App.h"

namespace Scene {

	bool Info::Scene::operator==(const Scene& that) const
	{
		return sky.sunRotation == that.sky.sunRotation &&
			numRays == that.numRays &&
			denoiseRadius == that.denoiseRadius &&
			windowSize == that.windowSize &&
			newRays == that.newRays;
	}

	void Info::Init()
	{
		buffer.Allocate(1);
		scene = {
			1, 0,
			App::app.graphics.window.windowSize,
			{ { 0.0f, 0.5f } }, TRUE
		};
		srv.Init(sizeof(Scene), 1, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		buffer[0] = scene;
		Upload();
	}

	void Info::Upload()
	{
		srv.Upload(buffer.GetResourceData(), App::app.graphics.GetCommandList());
	}

	void Info::Update()
	{
		if (scene != buffer[0]) {
			buffer[0] = scene;
			Upload();
		}
		scene.newRays = false;
	}

}