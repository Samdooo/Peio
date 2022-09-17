#include "..\App.h"

namespace Graphics {

	void Graphics::Init()
	{
		window.Init();

		Peio::Gfx::Init();
		graphics.Init(window.window.GetHWND(), window.windowSize, 2, false);

		rayTracer.Init();
		denoiser.Init();
		//gui.Init();
	}

	void Graphics::Update()
	{
		window.Update();
		gui.Update();

		graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

		rayTracer.Render();
		denoiser.Render();

		gui.Render();

		graphics.Render();
	}

	ID3D12GraphicsCommandList* Graphics::GetCommandList() const 
	{
		return graphics.GetCommandList();
	}

}



