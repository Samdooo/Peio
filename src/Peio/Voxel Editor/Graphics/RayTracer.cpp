#include "..\App.h"

namespace Graphics {

	void RayTracer::Init()
	{
		renderer.Init(App::app.graphics.GetCommandList(), "Resources/Shaders/RayVS.cso", "Resources/Shaders/RayPS.cso", {
				D3D12_SHADER_VISIBILITY_VERTEX, // Camera
				D3D12_SHADER_VISIBILITY_PIXEL, // Scene info
				D3D12_SHADER_VISIBILITY_PIXEL, // World
				D3D12_SHADER_VISIBILITY_PIXEL // Materials
			}, {
				D3D12_SHADER_VISIBILITY_PIXEL // Ray field
			});
	}

	void RayTracer::Render()
	{
		Peio::Uint2 windowSize = App::app.graphics.window.windowSize;

		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, (LONG)windowSize.x(), (LONG)windowSize.y() };
		renderer.Prepare(App::app.graphics.GetCommandList(), viewPort, scissorRect);

		UINT frameIndex = App::app.graphics.graphics.GetFrameIndex();
		UINT numBuffers = App::app.graphics.graphics.GetRenderTargets().GetNumBuffers();

		renderer.SetSRV(App::app.graphics.GetCommandList(), 0, App::app.scene.camera.srv.GetGPUAddress());
		renderer.SetSRV(App::app.graphics.GetCommandList(), 1, App::app.scene.info.srv.GetGPUAddress());
		renderer.SetSRV(App::app.graphics.GetCommandList(), 2, App::app.scene.world.mapSrv.GetGPUAddress());
		renderer.SetSRV(App::app.graphics.GetCommandList(), 3, App::app.scene.world.materialSrv.GetGPUAddress());
		renderer.SetUAV(App::app.graphics.GetCommandList(), 0, App::app.scene.rayField.uav.GetGPUAddress());

		renderer.Render(App::app.graphics.GetCommandList());
	}

}