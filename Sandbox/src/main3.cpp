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
#include <Peio/Voxels/DenoiseRenderer.h>

#include <map>
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

//void LoadBlock(std::string path, Peio::Uint3 pos, Peio::Vxl::MaterialMap& map, std::vector<Material>& materials) {
//
//}

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
		renderer.camera.fov = 1.65806f;
		renderer.camera.position = { 5.0f, 5.0f, -5.0f };

		renderer.scene.numRays = 1;
		renderer.scene.windowSize = (Peio::Float2)windowSize;

		renderer.materialMap.Init((1 << 14), 10);

		UINT rad = 16;
		
		std::vector<Material> materials = {};
		materials.push_back({
			{}, { 5.0f, 5.0f, 5.0f }, 0.0f
			});

		{
			std::unordered_map<UINT, std::string> blockPaths = {
				//{ 13, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/podzol_top.png" },
				//{ 11, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/coarse_dirt.png" },
				//{ 1, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/stone.png" },
				//{ 109, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/gravel.png" },
				//{ 121, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/spruce_log.png" },
				//{ 237, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/spruce_leaves.png" },
				//{ 10, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/dirt.png" }
				{ 9, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/grass_block_top.png" },
				{ 10, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/dirt.png" },
				{ 1, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/stone.png" },
				{ 109, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/gravel.png" },
				{ 209, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_leaves.png" },
				{ 225, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_leaves.png" },
				{ 221, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_leaves.png" },
				{ 118, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_log.png" },
				{ 273, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_leaves.png" },
				{ 269, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_leaves.png" },
				{ 277, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_leaves.png" },
				{ 124, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_log.png" },
			};

			std::unordered_map<UINT, UINT> matList = {};

			for (auto& blockPath : blockPaths) {
				Peio::Med::Frame img = Peio::Med::Images::Load(blockPath.second, AV_PIX_FMT_RGBA);
				for (UINT y = 0; y < 16; y++) {
					for (UINT x = 0; x < 16; x++) {
						if (matList.contains(*(UINT*)img.GetPixel(y, x)))
							continue;
						materials.push_back({ { (float)img.GetPixel(y, x)[0] / 255.0f, (float)img.GetPixel(y, x)[1] / 255.0f, (float)img.GetPixel(y, x)[2] / 255.0f, 1.0f}, {}, 0.0f});
						matList.insert({ *(UINT*)img.GetPixel(y, x), (UINT)materials.size() - 1 });
					}
				}
				std::cout << "Loaded " << blockPath.second << std::endl;
			}

			std::ifstream ifile("world.txt");
			if (!ifile.good()) {
				std::cout << "No world.txt" << std::endl;
				exit(0);
			}
			UINT sizeX, sizeY, sizeZ;
			ifile >> sizeX >> sizeY >> sizeZ;

			for (UINT x = 0; x < sizeX; x++) {
				std::cout << "Loading x " << (x + 1) << " / " << sizeX << std::endl;
				for (UINT y = 0; y < sizeY; y++) {
					for (UINT z = 0; z < sizeZ; z++) {
						UINT blockIndex;
						ifile >> blockIndex;
						if (!blockPaths.contains(blockIndex))
							continue;
						std::string path = blockPaths.at(blockIndex);
						//std::cout << "Loading " << blockIndex << std::endl;

						Peio::Med::Frame img = Peio::Med::Images::Load(path, AV_PIX_FMT_RGBA);
						for (UINT x2 = 0; x2 < 16; x2++) {
							for (UINT y2 = 0; y2 < 16; y2++) {
								for (UINT z2 = 0; z2 < 16; z2++) {
									if (!matList.contains(*(UINT*)img.GetPixel(z2, x2))) {
										std::cout << "Matlist didn't contain " << ((Peio::Byte4*)img.GetPixel(z2, x2))->ToString() << std::endl;
										exit(0);
									}
									renderer.materialMap.SetMaterial({ x * 16 + x2, y * 16 + y2, z * 16 + z2 },
										matList.at(*(UINT*)img.GetPixel(z2, x2)));
								}
							}
						}
						for (UINT x2 = 0; x2 < 16; x2++) {
							for (UINT y2 = 0; y2 < 16; y2++) {
								renderer.materialMap.SetMaterial({ x * 16 + x2, y * 16 + y2, z * 16 + 0 },
									matList.at(*(UINT*)img.GetPixel(y2, x2)));
								renderer.materialMap.SetMaterial({ x * 16 + x2, y * 16 + y2, z * 16 + 15 },
									matList.at(*(UINT*)img.GetPixel(y2, x2)));
								renderer.materialMap.SetMaterial({ x * 16 + 0, y * 16 + y2, z * 16 + x2 },
									matList.at(*(UINT*)img.GetPixel(y2, x2)));
								renderer.materialMap.SetMaterial({ x * 16 + 15, y * 16 + y2, z * 16 + x2 },
									matList.at(*(UINT*)img.GetPixel(y2, x2)));
							}
						}
					}
				}
			}
			ifile.close();
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
					//result.voxel[result.side] += (result.normal[result.side] > 0.0) ? 1 : 0;
					if (result.normal[result.side] > 0.0)
						result.voxel[result.side]++;
					else
						result.voxel[result.side]--;
					std::cout << "Placed at " << result.voxel.ToString() << std::endl;
					renderer.materialMap.SetMaterial(result.voxel, 0);
					renderer.UpdateMaterialMap(graphics.GetCommandList());
				}
				return false;
			}
		);
		Peio::Win::Input::eventHandlers.Insert(&mouseButtonDownHandler, mouseButtonDownHandler.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::Gfx::BufferUAV rayUav;
		rayUav.Init(sizeof(Peio::Vxl::VoxelRenderer::Ray)* windowSize.x()* windowSize.y(), windowSize.x()* windowSize.y(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		Peio::Vxl::DenoiseRenderer denoiser;
		denoiser.Init(graphics.GetCommandList());

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

			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect, &materialSrv, &rayUav);
			denoiser.Render(graphics.GetCommandList(), viewPort, scissorRect, renderer.GetSceneSrv(), &materialSrv, &rayUav);

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