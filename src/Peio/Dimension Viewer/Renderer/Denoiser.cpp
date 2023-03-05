#include "..\App.h"

#include <iostream>

namespace Renderer {

	void Denoiser::Init() 
	{
		if (numDims == App::app.scene.numDims)
			return;
		numDims = App::app.scene.numDims;
		std::vector<std::pair<std::string, std::string>> defines = {
			{ "numDims", std::to_string(numDims) }
		};
		Renderer::vs.Compile("Shaders/DenoiseVS.hlsl", "vs_6_0", defines);
		std::cout << "Compiled DenoiseVS.hlsl" << std::endl;
		Renderer::ps.Compile("Shaders/DenoisePS.hlsl", "ps_6_0", defines);
		std::cout << "Compiled DenoisePS.hlsl" << std::endl;

		FullscreenRenderer::Create();
		std::cout << "Created denoiser" << std::endl;
	}

}