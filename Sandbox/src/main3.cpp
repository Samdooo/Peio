#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>
#include <Peio/Windows/Window.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Graphics/TextureGraphics.h>
#include <Peio/Windows/RawMouseListener.h>
#include <Peio/Clock.h>
#include <Peio/Graphics/MultiPassGraphics.h>
#include <Peio/Graphics/UnorderedAccessView.h>
#include <Peio/Graphics/VertexBuffer.h>
#include <Peio/Graphics/PipelineState.h>
#include <Peio/Graphics/PipelineStateHeader.h>
#include <Peio/Graphics/ShaderResourceView.h>
#include <functional>
#include <Peio/Media/Images.h>
#include <Peio/Voxels/VoxelRenderer.h>

#include <unordered_map>

#define PI 3.14159265358979f
#define PHI 0.618033988749895f

bool Keydown(int key) {
	return GetKeyState(key) & 0x8000;
}

struct Material {
	Peio::Float4 color = {};
	Peio::Float3 light = {};
	float spread = 0.0f;
};

Peio::Float3 RotateX(Peio::Float3 p, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Peio::Float3(p.x(), c * p.y() + s * p.z(), -s * p.y() + c * p.z());
}

Peio::Float3 RotateY(Peio::Float3 p, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	return Peio::Float3(c * p.x() - s * p.z(), p.y(), s * p.x() + c * p.z());
}

Peio::Double3 GetRay(Peio::Double2 rotation) {
	Peio::Double3 result = { 0.0, 0.0, 1.0 };
	result = RotateX(result, rotation.y());
	result = RotateY(result, rotation.x());
	return result;
}

int main() {

	try {

		Peio::Gfx::Init();

		Peio::Int2 windowSize = { 1280, 720 };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };

		Peio::Win::Window window;
		window.CreateClass("Peio Window Class", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, windowSize);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);
		
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		
		RECT clip = {
			100, 100, 1280 + 100, 720 + 100
		};
		ClipCursor(&clip);

		Peio::Vxl::VoxelRenderer renderer;
		renderer.camera.aspectRatio = (float)windowSize.y() / (float)windowSize.x();
		renderer.camera.fov = 1.5f;
		renderer.camera.position = { 5.0f, 5.0f, -5.0f };

		renderer.scene.numRays = 2;
		renderer.scene.windowSize = (Peio::Float2)windowSize;

		UINT rad = 16;
		
		Peio::Med::Frame img = Peio::Med::Images::Load("D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/ancient_debris_side.png"
			, AV_PIX_FMT_RGBA);
		std::vector<Material> materials = {};
		for (UINT y = 0; y < rad; y++) {
			for (UINT x = 0; x < rad; x++) {
				materials.push_back({ { 
						(float)img.GetPixel(y, x)[0] / 255.0f,
						(float)img.GetPixel(y, x)[1] / 255.0f,
						(float)img.GetPixel(y, x)[2] / 255.0f,
						1.0f
					}, {}, 0.0f});
			}
		}

		renderer.materialMap.Init((1 << 20), 24);
		for (UINT x = 0; x < rad; x++) {
			for (UINT y = 0; y < rad; y++) {
				for (UINT z = 0; z < rad; z++) {
					renderer.materialMap.SetMaterial({ x, y, z }, (x * rad) + z);
				}
			}
		}
		for (UINT y = 0; y < rad; y++) {
			for (UINT x = 0; x < rad; x++) {
				renderer.materialMap.SetMaterial({ 0, y, x }, (y * rad) + x);
				renderer.materialMap.SetMaterial({ rad - 1, y, x }, (y * rad) + x);
				renderer.materialMap.SetMaterial({ x, y, 0 }, (y * rad) + x);
				renderer.materialMap.SetMaterial({ x, y, rad - 1 }, (y * rad) + x);
			}
		}
		
		renderer.Init(graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.SetBuffer(&materials[0], materials.size());
		
		Peio::Gfx::BufferSRV materialSrv;
		materialSrv.Init(sizeof(Material) * materials.size(), materials.size(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		materialSrv.Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());

		float acceleration = 1.1f;
		float retardation = 0.1f;
		Peio::Float3 velocity = {};

		Peio::FunctionHandler<Peio::Win::RawMouseMoveEvent> mouseMoveHandler(
			[&renderer](Peio::Win::RawMouseMoveEvent* event) -> bool {
				renderer.camera.rotation.x() -= (float)event->movement.x() / 500.0f;
				renderer.camera.rotation.y() -= (float)event->movement.y() / 500.0f;
				return false;
			}
		);
		Peio::Win::RawMouseListener rawMouseListener;
		Peio::Win::RawMouseListener::Register(window.GetHWND());
		Peio::Win::Input::AddListener(&rawMouseListener);

		Peio::Win::Input::eventHandlers.Insert(&mouseMoveHandler, mouseMoveHandler.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::FunctionHandler<Peio::Win::RawMouseButtonDownEvent> mouseButtonDownHandler(
			[&renderer, &graphics](Peio::Win::RawMouseButtonDownEvent* event) -> bool {
				if (event->button == Peio::Win::MouseButton::RIGHT && event->foreground) {
					Peio::Double3 ray = GetRay(renderer.camera.rotation);
					Peio::Vxl::MaterialMap::Ray result = renderer.materialMap.Trace((Peio::Double3)renderer.camera.position, ray);
					if (result.material == ~0) {
						std::cout << "Traced air" << std::endl;
						return false;
					}
					result.voxel[result.side] += (result.normal[result.side] > 0.0) ? 1 : 0;
					std::cout << "Placed at " << result.voxel.ToString() << std::endl;
					renderer.materialMap.SetMaterial(result.voxel, 0);
					renderer.UpdateMaterialMap(graphics.GetCommandList());
				}
				return false;
			}
		);
		Peio::Win::Input::eventHandlers.Insert(&mouseButtonDownHandler, mouseButtonDownHandler.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::Clock<double> deltaClock;

		while (true) {
			
			window.HandleMessages();

			double deltaTime = deltaClock.Restart().Seconds();

			if (Keydown(VK_CONTROL)) {
				acceleration = 50.0f;
			}
			else {
				acceleration = 5.0f;
			}

			if (Keydown('W')) {
				velocity.x() += (float)deltaTime * acceleration * -sin(renderer.camera.rotation.x());
				velocity.z() += (float)deltaTime * acceleration * cos(renderer.camera.rotation.x());
			}
			if (Keydown('S')) {
				velocity.x() -= (float)deltaTime * acceleration * -sin(renderer.camera.rotation.x());
				velocity.z() -= (float)deltaTime * acceleration * cos(renderer.camera.rotation.x());
			}
			if (Keydown('D')) {
				velocity.x() += (float)deltaTime * acceleration * cos(renderer.camera.rotation.x());
				velocity.z() += (float)deltaTime * acceleration * sin(renderer.camera.rotation.x());
			}
			if (Keydown('A')) {
				velocity.x() -= (float)deltaTime * acceleration * cos(renderer.camera.rotation.x());
				velocity.z() -= (float)deltaTime * acceleration * sin(renderer.camera.rotation.x());
			}
			if (Keydown(VK_SPACE)) {
				velocity.y() += (float)deltaTime * acceleration;
			}
			if (Keydown(VK_SHIFT)) {
				velocity.y() -= (float)deltaTime * acceleration;
			}

			renderer.camera.position += velocity;
			velocity -= velocity * retardation;

			renderer.UpdateCamera(graphics.GetCommandList());

			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect, &materialSrv);

			graphics.Render();

		}

	}
	catch (Peio::Gfx::Exception exception) {
		std::cout << "GfxException: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << " ret: " << exception.ret << "." << std::endl;
	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}