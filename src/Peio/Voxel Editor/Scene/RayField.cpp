#include "..\App.h"
#include <random>

namespace Scene {

	void RayField::Init()
	{
		Peio::Uint2 windowSize = App::app.graphics.window.windowSize;
		buffer.Allocate(windowSize.x() * windowSize.y());

		uav.Init(buffer.GetBufferSize(), buffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		std::uniform_int_distribution<UINT> random(0, (1 << 16));
		std::default_random_engine randomGen = {};

		for (UINT y = 0; y < windowSize.y(); y++) {
			for (UINT x = 0; x < windowSize.x(); x++) {
				buffer[y * windowSize.x() + x].seed = random(randomGen);
			}
		}
		Upload();
	}

	void RayField::Upload()
	{
		uav.Upload(buffer.GetResourceData(), App::app.graphics.GetCommandList());
	}

}