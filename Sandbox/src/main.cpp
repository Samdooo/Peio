#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>
#include <Peio/Windows/Window.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Voxels/VoxelRenderer.h>
#include <Peio/Voxels/VoxelScene.h>

#include <unordered_map>

struct Handler : public Peio::EventHandler<Peio::Win::KeydownEvent, Peio::Win::KeyupEvent> {

	void Handle(Peio::Win::KeydownEvent& event) {
		if (!event.prev)
			std::cout << event.key << " pressed" << std::endl;
	}

	void Handle(Peio::Win::KeyupEvent& event) {
		std::cout << event.key << " released" << std::endl;
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

		Peio::Win::KeyboardListener listener;
		Peio::Win::Input::AddListener(&listener);

		Handler handler;
		Peio::Win::Input::AddEventHandler<Peio::Win::KeydownEvent, Peio::Win::KeyupEvent>(&handler);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), { 1280, 720 }, 3, false);

		Peio::Vxl::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = { 0, 0, 0.5f, 16, 16, 0.01f, 1 };

		//Peio::Vxl::SubresourceBuffer<Material> materialBuffer;


		//Peio::Gfx::ShaderResourceView srv;
		//srv.InitBuffer(
		//	{  }
		//);

		Peio::Vxl::VoxelRenderer renderer;
		//renderer.Init(graphics.GetCommandList(), )

		while (true) {
			window.HandleMessages();

			graphics.Clear({ 1.0f, 0.0f, 0.0f, 1.0f });



			graphics.Render();
		}

	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}