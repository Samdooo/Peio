#include "..\App.h"

namespace Graphics {

	void Denoiser::Init()
	{
		renderer.Init(App::app.graphics.GetCommandList(), "Resources/Shaders/DenoiseVS.cso", "Resources/Shaders/DenoisePS.cso", {
				D3D12_SHADER_VISIBILITY_PIXEL, // Scene info
				D3D12_SHADER_VISIBILITY_PIXEL // Materials
			}, {
				D3D12_SHADER_VISIBILITY_PIXEL // Ray field
			});
	}

	void Denoiser::Render()
	{
		Peio::Uint2 windowSize = App::app.graphics.window.windowSize;

		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, (LONG)windowSize.x(), (LONG)windowSize.y() };
		renderer.Prepare(App::app.graphics.GetCommandList(), viewPort, scissorRect);

		UINT frameIndex = App::app.graphics.graphics.GetFrameIndex();

		renderer.SetSRV(App::app.graphics.GetCommandList(), 0, App::app.scene.info.srv.GetGPUAddress());
		renderer.SetSRV(App::app.graphics.GetCommandList(), 1, App::app.scene.world.materialSrv.GetGPUAddress());
		renderer.SetUAV(App::app.graphics.GetCommandList(), 0, App::app.scene.rayField.uav.GetGPUAddress());

		renderer.Render(App::app.graphics.GetCommandList());
	}

}