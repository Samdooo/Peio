#include "..\App.h"

#include <iostream>

namespace Renderer {

	void RayRenderer::Init()
	{
		if (numDims == App::app.scene.numDims && maxBounces == App::app.scene.maxBounces)
			return;
		numDims = App::app.scene.numDims;
		maxBounces = App::app.scene.maxBounces;
		std::vector<std::pair<std::string, std::string>> defines = {
			{ "numDims", std::to_string(numDims) },
			{ "maxBounces", std::to_string(maxBounces) }
		};
		Renderer::vs.Compile("Shaders/RayVS.hlsl", "vs_6_0", defines);
		std::cout << "Compiled RayVS.hlsl" << std::endl;
		Renderer::ps.Compile("Shaders/RayPS.hlsl", "ps_6_0", defines);
		std::cout << "Compiled RayPS.hlsl" << std::endl;

		FullscreenRenderer::Create();
		std::cout << "Created ray renderer" << std::endl;
	}

}