#include <iostream>
#include <Peio/Exception.h>

#include <Peio/Networking/Server.h>
#include <Peio/Networking/Init.h>
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
#include <sstream>

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

using uint = UINT;

template <typename T, size_t length>
struct std::hash<Peio::Array<T, length>> {

	size_t operator()(const Peio::Array<T, length>& array) const {
		size_t hash = 0;
		for (size_t i = 0; i < length; i++)
			hash ^= std::hash<T>()(array[i]);
		return hash;
	}

};

int main() {

	//Peio::Net::Init();
	//
	//char* buffer = new char[(1 << 24)];
	//int bufferOffset = 0;
	//
	//Peio::FunctionHandler<Peio::Net::ServerConnectionEvent, Peio::Net::ServerReceiveEvent, Peio::Net::ServerDisconnectionEvent> handler(
	//	[](Peio::Net::ServerConnectionEvent* event) -> bool {
	//		std::cout << event->client->GetGlobal().GetIp() << " connected" << std::endl;
	//	},
	//	[buffer, &bufferOffset](Peio::Net::ServerReceiveEvent* event) -> bool {
	//		//std::cout << event->client->GetGlobal().GetIp() << " sent:" << std::endl;
	//		//std::cout << std::string(event->buffer, event->length) << std::endl;
	//
	//		memcpy(buffer + bufferOffset, event->buffer, event->length);
	//		bufferOffset += event->length;
	//
	//		if (bufferOffset == sizeof(Peio::Int2)) {
	//			bufferOffset = 0;
	//
	//			char ok = 1;
	//			event->client->Send(&ok, 1);
	//
	//			Peio::Int2 pos = *(Peio::Int2*)buffer;
	//			std::cout << "Received chunk " << pos.ToString() << std::endl;
	//		}
	//	},
	//	[](Peio::Net::ServerDisconnectionEvent* event) -> bool {
	//		std::cout << event->client->GetGlobal().GetIp() << " disconnected" << std::endl;
	//	}
	//);
	//
	//Peio::Net::Server server;
	//server.Init(&handler, (1 << 24), 14000);
	//server.Listen();
	//
	//while (true) {
	//	server.Update();
	//}

	//Peio::Net::Init();
	//Peio::Net::ListenerSocket listener;
	//listener.Init();
	//Peio::Net::Hint listenerHint;
	//listenerHint.Init(14000);
	//listener.Bind(listenerHint);
	////listener.RegisterEvents();
	//listener.Listen();
	//
	//


	//IndexMap<uint, 3> map;
	//map.Init(3, 16);
	//
	//map.SetIndex({ 0, 0, 0 }, 69);
	//map.SetIndex({ 0, 0, 1 }, 70);
	//map.SetIndex({ 2, 0, 0 }, 69);
	//map.SetIndex({ 2, 0, 1 }, 70);
	//
	//map.SetIndex({ 0, 4, 0 }, 69);
	//map.SetIndex({ 0, 4, 1 }, 70);
	//map.SetIndex({ 2, 4, 0 }, 69);
	//map.SetIndex({ 2, 4, 1 }, 70);
	////map.SetIndex({ 2, 4, 0 }, 71);
	////map.SetIndex({ 2, 4, 1 }, 72);
	//
	//for (uint i = 0; i < map.nodes.size(); i++) {
	//	std::cout << "Node #" << i << ": ";
	//	for (uint x = 0; x < 2; x++) {
	//		for (uint y = 0; y < 2; y++) {
	//			for (uint z = 0; z < 2; z++) {
	//				std::cout << map.nodes[i][x][y][z] << " ";
	//			}
	//		}
	//	}
	//	std::cout << "References: " << map.references[i] << std::endl;
	//}

	//std::cout << t.Hash() << std::endl;

	//Peio::Vxl::MaterialMap map;
	//map.Init(16, 4);
	//
	//uint rad = 8;
	//for (uint x = 0; x < rad; x++) {
	//	for (uint y = 0; y < rad; y++) {
	//		for (uint z = 0; z < rad; z++) {
	//			map.SetMaterial({ x, y, z }, 10);
	//		}
	//	}
	//}
	//for (uint x = 0; x < rad; x++) {
	//	for (uint y = 0; y < rad; y++) {
	//		for (uint z = 0; z < rad; z++) {
	//			map.SetMaterial({ x, y, z }, 11);
	//		}
	//	}
	//}
	//for (uint x = 0; x < rad / 2; x++) {
	//	for (uint y = 0; y < rad / 2; y++) {
	//		for (uint z = 0; z < rad / 2; z++) {
	//			map.SetMaterial({ x, y, z }, 12);
	//		}
	//	}
	//}
	//
	//for (UINT i = 0; i < map.groups.size(); i++) {
	//	std::cout << "Group #" << i << " ";
	//	for (UINT x = 0; x < 2; x++)
	//		for (UINT y = 0; y < 2; y++)
	//			for (UINT z = 0; z < 2; z++)
	//				std::cout << map.groups[i].indices[x][y][z] << " ";
	//	std::cout << "Refs: " << map.references[i] << std::endl;
	//}
	//return 0;

	try {
	
		Peio::Net::Init();
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

		renderer.scene.numRays = 4;
		renderer.scene.windowSize = (Peio::Float2)windowSize;

		renderer.materialMap.Init(16, (1 << 22));

		UINT rad = 16;

		std::vector<Material> materials = {};
		materials.push_back({
			{}, { 5.0f, 5.0f, 5.0f }, 0.0f
			});

		std::unordered_map<int, std::string> blockPaths = {
			{ 1, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/stone.png" },
			{ 2, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/dirt.png" },
			{ 3, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/grass_block_top.png" },
			{ 4, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/snow.png" },
			{ 5, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_log.png" },
			{ 6, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_leaves.png" },
			{ 7, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_log.png" },
			{ 8, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/birch_leaves.png" },
			{ 9, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/spruce_log.png" },
			{ 10, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/spruce_leaves.png" },
			{ 11, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/cobblestone.png" },
			{ 12, "D:/Users/Samdo/C++/Peio Sandbox Resources/1.19/assets/minecraft/textures/block/oak_planks.png" }
		};

		std::unordered_map<UINT, UINT> matList = {};

		for (auto& blockPath : blockPaths) {
			Peio::Med::Frame img = Peio::Med::Images::Load(blockPath.second, AV_PIX_FMT_RGBA);
			for (UINT y = 0; y < 16; y++) {
				for (UINT x = 0; x < 16; x++) {
					if (matList.contains(*(UINT*)img.GetPixel(y, x)))
						continue;
					materials.push_back({ { (float)img.GetPixel(y, x)[0] / 255.0f, (float)img.GetPixel(y, x)[1] / 255.0f, (float)img.GetPixel(y, x)[2] / 255.0f, 1.0f}, {}, 0.0f });
					matList.insert({ *(UINT*)img.GetPixel(y, x), (UINT)materials.size() - 1 });
				}
			}
			std::cout << "Loaded " << blockPath.second << std::endl;
		}

		std::unordered_map<int, UINT> blockMap = {};

		char* serverBuffer = new char[(1 << 24)];
		int serverBufferOffset = 0;

		Peio::FunctionHandler<Peio::Net::ServerConnectionEvent, Peio::Net::ServerReceiveEvent, Peio::Net::ServerDisconnectionEvent> serverHandler(
			[](Peio::Net::ServerConnectionEvent* event) -> bool {
				std::cout << event->client->GetGlobal().GetIp() << " connected" << std::endl;
				return false;
			},
			[serverBuffer, &serverBufferOffset, &blockMap, &blockPaths, &renderer, &matList, &graphics](Peio::Net::ServerReceiveEvent* event) -> bool {
				//std::cout << event->client->GetGlobal().GetIp() << " sent:" << std::endl;
				//std::cout << std::string(event->buffer, event->length) << std::endl;

				memcpy(serverBuffer + serverBufferOffset, event->buffer, event->length);
				serverBufferOffset += event->length;

				if (serverBufferOffset == sizeof(int) * (2 + (16 * 16 * 256))) {
					serverBufferOffset = 0;

					Peio::Int2 chunkPos = *(Peio::Int2*)serverBuffer;
					if (chunkPos.x() < 0 || chunkPos.y() < 0) {
						std::cout << "Received invalid chunk " << chunkPos.ToString() << std::endl;
						char ok = 1;
						event->client->Send(&ok, 1);
						return false;
					}

					std::cout << "Receiving chunk " << chunkPos.ToString() << std::endl;

					for (int by = 0; by < 256; by++) {
						for (int bx = 0; bx < 16; bx++) {
							for (int bz = 0; bz < 16; bz++) {
								int vx = (chunkPos.x() * 16 + bx) * 16;
								int vy = by * 16;
								int vz = (chunkPos.y() * 16 + bz) * 16;

								int key = ((int*)serverBuffer)[2 + ((by * 16 + bx) * 16 + bz)];
								if (key == 0)
									continue;
								if (!blockPaths.contains(key))
									continue;
								if (blockMap.contains(key)) {
									UINT ind = blockMap.at(key);
									renderer.materialMap.SetIndex({ (UINT)vx, (UINT)vy, (UINT)vz }, ind, renderer.materialMap.numLayers - 1 - 4);
								}
								else {
									Peio::Med::Frame img = Peio::Med::Images::Load(blockPaths.at(key), AV_PIX_FMT_RGBA);

									for (int x = 0; x < 16; x++) {
										for (int y = 0; y < 16; y++) {
											for (int z = 0; z < 16; z++) {
												//UINT material = ~0;
												UINT col = 0;
												if (x == 0 || x == 15)
													//material = matList.at(*(UINT*)img.GetPixel(y, z));
													col = *(UINT*)img.GetPixel(y, z);
												else if (z == 0 || z == 15)
													col = *(UINT*)img.GetPixel(y, x);
												else if (y == 0 || y == 15)
													col = *(UINT*)img.GetPixel(z, x);
												else
													col = (rand() % 2) ? *(UINT*)img.GetPixel(z, x) : *(UINT*)img.GetPixel(y, x);
												if (((byte*)&col)[3] == 0)
													continue;
												UINT material = matList.at(col);
												renderer.materialMap.SetIndex({ (UINT)vx + (UINT)x, (UINT)vy + (UINT)y, (UINT)vz + (UINT)z }, material);
											}
										}
									}
									blockMap.insert({ key, renderer.materialMap.GetIndex({ (UINT)vx, (UINT)vy, (UINT)vz }, renderer.materialMap.numLayers - 1 - 4) });
								}
							}
						}
					}
					renderer.UpdateMaterialMap(graphics.GetCommandList());

					std::cout << "Successfully received chunk " << chunkPos.ToString() << std::endl;

					char ok = 1;
					event->client->Send(&ok, 1);
				}
				return false;
			},
			[](Peio::Net::ServerDisconnectionEvent* event) -> bool {
				std::cout << event->client->GetGlobal().GetIp() << " disconnected" << std::endl;
				return false;
			}
		);

		Peio::Net::Server server;
		server.Init(&serverHandler, (1 << 24), 14000);
		server.Listen();

		renderer.Init(graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.SetBuffer(&materials[0], materials.size());

		Peio::Gfx::BufferSRV materialSrv;
		materialSrv.Init(sizeof(Material)* materials.size(), materials.size(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		materialSrv.Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());

		float acceleration = 1.1f;
		float retardation = 0.1f;
		Peio::Float3 velocity = {};

		Peio::FunctionHandler<Peio::Win::RawMouseMoveEvent> mouseMoveHandler(
			[&renderer](Peio::Win::RawMouseMoveEvent* event) -> bool {
				renderer.camera.rotation.x() -= (float)event->movement.x() / 1450.0f;
				renderer.camera.rotation.y() -= (float)event->movement.y() / 1450.0f;
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
					int rad = 5;
					for (int x = -rad; x <= rad; x++) {
						for (int y = -rad; y <= rad; y++) {
							for (int z = -rad; z <= rad; z++) {
								double dist = sqrt(x * x + y * y + z * z);
								if (dist < (double)rad)
									renderer.materialMap.SetIndex((Peio::Int3)result.voxel + Peio::Int3(x, y, z), 1);
							}
						}
					}
					renderer.UpdateMaterialMap(graphics.GetCommandList());
				}
				else if (event->button == Peio::Win::MouseButton::LEFT && event->foreground) {
					Peio::Double3 ray = GetRay(renderer.camera.rotation);
					Peio::Vxl::MaterialMap::Ray result = renderer.materialMap.Trace((Peio::Double3)renderer.camera.position, ray);
					if (result.material == ~0) {
						std::cout << "Traced air" << std::endl;
						return false;
					}
					int rad = 5;
					for (int x = -rad; x <= rad; x++) {
						for (int y = -rad; y <= rad; y++) {
							for (int z = -rad; z <= rad; z++) {
								double dist = sqrt(x * x + y * y + z * z);
								if (dist < (double)rad)
									renderer.materialMap.SetIndex((Peio::Int3)result.voxel + Peio::Int3(x, y, z), Peio::Vxl::MaterialMap::null);
							}
						}
					}
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
		uint frameCount = 0;
		double fpsTime = 0.0;
		double minFrameTime = 1.0 / 40.0;

		while (true) {

			server.Update();

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

			if (Keydown(VK_UP)) {
				renderer.scene.sky.sunRotation.y() += 0.04f;
			}
			if (Keydown(VK_DOWN)) {
				renderer.scene.sky.sunRotation.y() -= 0.04f;
			}

			renderer.camera.position += velocity;
			velocity -= velocity * retardation;

			renderer.UpdateScene(graphics.GetCommandList());
			renderer.UpdateCamera(graphics.GetCommandList());

			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect, &materialSrv, &rayUav);
			denoiser.Render(graphics.GetCommandList(), viewPort, scissorRect, renderer.GetSceneSrv(), &materialSrv, &rayUav);

			graphics.Render();

			while (deltaClock.Elapsed().Seconds() < minFrameTime);

			frameCount++;
			fpsTime += deltaTime;

			if (fpsTime >= 1.0) {
				fpsTime -= 1.0;
				std::cout << "FPS: " << frameCount << std::endl;
				frameCount = 0;
			}
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