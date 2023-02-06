#include "..\App.h"

namespace Renderer {

	void RayRenderer::Init()
	{
		std::vector<std::pair<std::string, std::string>> defines = {
			{ "numDims", std::to_string(App::app.scene.numDims) },
			{ "maxBounces", std::to_string(App::app.scene.maxBounces) }
		};
		Renderer::vs.Compile("../src/Peio/Dimension Viewer/Renderer/Shaders/RayVS.hlsl", "vs_6_0", defines);
		Renderer::ps.Compile("../src/Peio/Dimension Viewer/Renderer/Shaders/RayPS.hlsl", "ps_6_0", defines);
		FullscreenRenderer::Create();
	}

}