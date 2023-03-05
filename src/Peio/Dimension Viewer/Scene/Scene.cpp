#include "..\App.h"

#include <iostream>

namespace Scene {

	void Scene::Init() 
	{
		if (!App::app.init) {
			infoBuffer.Init(sizeof(Info), 1, false);
			infoBuffer.buffer.SetData((byte*)&info, sizeof(Info));
			infoBuffer.shaderRegister = 0;
			infoBuffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
			App::app.rayRenderer.parameters.push_back(&infoBuffer);
			App::app.denoiser.parameters.push_back(&infoBuffer);
			std::cout << "Initialized scene buffer" << std::endl;
		}
		camera.Init();
		objects.Init();
		rays.Init();
	}

	void Scene::Update(ID3D12GraphicsCommandList* cmdList)
	{
		info.numRects = (UINT)objects.hyperRectangles.rects.size();
		info.numSpheres = (UINT)objects.hyperSpheres.spheres.size();
		info.numRandom = rays.numRandom;
		infoBuffer.Upload(cmdList);

		camera.Update(cmdList);
		objects.Update(cmdList);
		rays.Update(cmdList);
	}

}