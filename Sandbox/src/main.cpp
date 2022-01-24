#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>
#include <Peio/Windows/Window.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Voxels/VoxelRenderer.h>
#include <Peio/Voxels/VoxelScene.h>
#include <Peio/Windows/RawMouseListener.h>
#include <Peio/Clock.h>

#include <unordered_map>

#define PI 3.14159265358979f

bool Keydown(int key) {
	return GetKeyState(key) & 0x8000;
}

struct Handler : public Peio::EventHandler<Peio::Win::RawMouseMoveEvent> {

	Peio::Float2* rotation = nullptr;

	void Handle(Peio::Win::RawMouseMoveEvent& event) override {
		if (!Keydown(VK_CONTROL)) {
			*rotation -= (Peio::Float2)event.movement / 1000.0f;
			SetCursorPos(200, 200);
		}
	}

};

struct Material {

	Peio::Float4 colorEmission;
	Peio::Float3 lightEmission;
	float lightSpread;

};

int main() {

	try {

		Peio::Gfx::Init();

		Peio::Win::Window window;
		window.CreateClass("Peio Sandbox", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { CW_USEDEFAULT, CW_USEDEFAULT }, { 1280, 720 });

		window.Show();
		ShowCursor(FALSE);

		Peio::Win::RawMouseListener listener;
		listener.Register(window.GetHWND());
		Peio::Win::Input::AddListener(&listener);

		Handler handler;
		Peio::Win::Input::AddEventHandler(&handler);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), { 1280, 720 }, 3, false);

		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, 1280, 720 };

		Peio::Vxl::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = { 0, 0, 0.5f, 16, 16, 0.01f, 1 };

		Peio::Gfx::ShaderResourceView srv;
		srv.InitBuffer(
			{ sizeof(Peio::Vxl::VoxelScene) },
			{ 1 },
			{ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE }
		);
		srv.GetResources()[0].Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Vxl::VoxelRenderer renderer;
		renderer.Init(graphics.GetCommandList(), &srv, {}, {}, PI / 2, 720.0f / 1280.0f);

		Peio::Float2 cameraRotation = {};
		handler.rotation = &cameraRotation;

		Peio::Clock<double> clock;
		double frameLength = 1.0f / 30.0;

		Peio::Clock<double> fpsClock;
		int frameCount = 0;

		while (true) {
			window.HandleMessages();

			renderer.SetCameraRotation(cameraRotation);
			renderer.UpdateCamera(graphics.GetCommandList());

			graphics.Clear({ 1.0f, 0.0f, 0.0f, 1.0f });

			renderer.Draw(graphics.GetCommandList(), viewPort, scissorRect);

			graphics.Render();

			while (clock.Elapsed().Seconds() < frameLength);
			clock.Restart();

			frameCount++;
			if (fpsClock.Elapsed().Seconds() >= 1.0) {
				std::cout << "FPS: " << ((double)frameCount / fpsClock.Restart().Seconds()) << std::endl;
				frameCount = 0;
			}
		}

	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}