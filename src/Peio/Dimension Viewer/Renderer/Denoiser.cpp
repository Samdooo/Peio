#include "..\App.h"

namespace Renderer {

	void Denoiser::Init() 
	{
		std::vector<std::pair<std::string, std::string>> defines = {
			{ "numDims", std::to_string(App::app.scene.numDims) }
		};
		Renderer::vs.Compile("C:/dev/C++/Peio/src/Peio/Dimension Viewer/Renderer/Shaders/DenoiseVS.hlsl", "vs_6_0", defines);
		Renderer::ps.Compile("C:/dev/C++/Peio/src/Peio/Dimension Viewer/Renderer/Shaders/DenoisePS.hlsl", "ps_6_0", defines);
		FullscreenRenderer::Create();
	}

}