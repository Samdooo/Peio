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

struct Camera {
	Peio::Float3 position = {};
	Peio::Float2 rotation = {};
	float fov = 0.0f;

	Peio::Float3 velocity = {};
};

#define GOLDEN_ANGLE 2.399963229728653f

int main() {

	try {

		Peio::Gfx::Init();

		Peio::Int2 windowSize = { 640, 360 };

		Peio::Win::Window window;
		window.CreateClass("Peio Sandbox", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { CW_USEDEFAULT, CW_USEDEFAULT }, windowSize);

		window.Show();
		ShowCursor(FALSE);

		Peio::Win::RawMouseListener listener;
		listener.Register(window.GetHWND());
		Peio::Win::Input::AddListener(&listener);

		Handler handler;
		Peio::Win::Input::AddEventHandler(&handler);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f};
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };

		Peio::Vxl::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = { 100, 0, 0.5f, 64, 32, 0.01f, 1.0f };

		Peio::Vxl::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.Allocate(1);
		materialBuffer.GetSubresourceBuffer()[0] = { { 0.0f, 0.5f, 0.7f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 1.0f };

		Peio::Vxl::SubresourceBuffer<Peio::Float3> voxelPositionBuffer;
		voxelPositionBuffer.Allocate(100);
		
		{
			float rad = 5.0f;
			for (int i = 0; i < 100; i++) {
				float radius = (float)i / 100.0f * rad;
				float y = rad - (float)i / 100.0f * rad;
				float x = cos(GOLDEN_ANGLE * i) * radius;
				float z = sin(GOLDEN_ANGLE * i) * radius;
				voxelPositionBuffer.GetSubresourceBuffer()[i] = { x, y, z };
			}
		}

		Peio::Vxl::SubresourceBuffer<UINT> voxelMaterialBuffer;
		voxelMaterialBuffer.Allocate(100);
		for (int i = 0; i < 100; i++)
		voxelMaterialBuffer.GetSubresourceBuffer()[i] = { 0 };

		Peio::Gfx::ShaderResourceView srv;
		srv.InitBuffer(
			{ sizeof(Peio::Vxl::VoxelScene), sizeof(Material), sizeof(Peio::Float3) * 100, sizeof(UINT) * 100 },
			{ 1, 1, 100, 100 },
			{ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE }
		);
		srv.GetResources()[0].Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[1].Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[2].Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[3].Upload(voxelMaterialBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Vxl::VoxelRenderer renderer;
		renderer.Init(graphics.GetCommandList(), &srv, {}, {}, PI / 2, (float)windowSize.y() / (float)windowSize.x());

		Camera camera = {};
		handler.rotation = &camera.rotation;

		Peio::Clock<double> clock;
		double frameLength = 1.0f / 30.0;

		Peio::Clock<double> fpsClock;
		int frameCount = 0;

		Peio::Clock<double> deltaClock;

		float acceleration = 1.0f;
		float retardation = 0.2f;

		while (true) {
			window.HandleMessages();
			double deltaTime = deltaClock.Restart().Seconds();

			if (Keydown('W')) {
				camera.velocity.x() += (float)deltaTime * acceleration * -sin(camera.rotation.x());
				camera.velocity.z() += (float)deltaTime * acceleration * cos(camera.rotation.x());
			}
			if (Keydown('S')) {
				camera.velocity.x() -= (float)deltaTime * acceleration * -sin(camera.rotation.x());
				camera.velocity.z() -= (float)deltaTime * acceleration * cos(camera.rotation.x());
			}
			if (Keydown('D')) {
				camera.velocity.x() += (float)deltaTime * acceleration * cos(camera.rotation.x());
				camera.velocity.z() += (float)deltaTime * acceleration * sin(camera.rotation.x());
			}
			if (Keydown('A')) {
				camera.velocity.x() -= (float)deltaTime * acceleration * cos(camera.rotation.x());
				camera.velocity.z() -= (float)deltaTime * acceleration * sin(camera.rotation.x());
			}
			if (Keydown(VK_SPACE)) {
				camera.velocity.y() += (float)deltaTime * acceleration;
			}
			if (Keydown(VK_SHIFT)) {
				camera.velocity.y() -= (float)deltaTime * acceleration;
			}
			camera.position += camera.velocity;

			camera.velocity -= camera.velocity * retardation;
			//camera.velocity -= camera.velocity * (retardation / (float)deltaTime);

			renderer.SetCameraPosition(camera.position);
			renderer.SetCameraRotation(camera.rotation);
			renderer.UpdateCamera(graphics.GetCommandList());

			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

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