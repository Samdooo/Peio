#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>
#include <Peio/Windows/Window.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Graphics/TextureGraphics.h>
#include <Peio/Voxels/VoxelRenderer.h>
#include <Peio/Voxels/VoxelScene.h>
#include <Peio/Windows/RawMouseListener.h>
#include <Peio/Clock.h>
#include <Peio/Voxels/DenoiseRenderer.h>
#include <Peio/Graphics/MultiPassGraphics.h>
#include <Peio/Voxels/PositionTree.h>

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

void PrintDepth(int depth) {
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
}

void Print(const Peio::Vxl::PositionTree& tree) {
	UINT layerSize = tree.GetNumChildren();
	for (size_t i = 0; i < tree.GetNumLayers() - 1; i++) {
		std::cout << "{  ";
		for (size_t j = 0; j < layerSize; j++) {
			std::cout << "[ " << tree.GetBranches()[i][j].descriptor << " ]" << "  ";
		}
		std::cout << "}" << std::endl;
		layerSize *= tree.GetNumChildren();
	}
	for (size_t i = 0; i < layerSize; i++) {
		std::cout << tree.GetLeaves()[i].index << "  ";
	}
	std::cout << std::endl;
}

struct PositionTree : public Peio::Vxl::PositionTree {

	Peio::Float3* voxelPositions = nullptr;

	Peio::Array<Peio::Float3, 2> GetBoundaries(UINT index) const override {
		return { voxelPositions[index] - 0.5f, voxelPositions[index] + 0.5f };
	}

};

int main() {

	std::vector<Peio::Float3> voxelPositions = {
		{ 1.0f, 1.0f, 1.0f },
		{ 2.0f, 2.0f, 2.0f },
		{ 3.0f, 3.0f, 3.0f },
		{ 4.0f, 4.0f, 4.0f },
		{ 5.0f, 6.0f, 7.0f },
		{ 6.0f, 5.0f, 4.0f },
	};

	PositionTree tree;
	tree.voxelPositions = &voxelPositions[0];

	tree.Allocate(3, 2);
	for (UINT i = 0; i < tree.GetNumBranches(); i++)
		tree.GetBranches()[0][i] = { 0, Peio::Array<Peio::Float3, 2>{ Peio::Float3{ D3D12_FLOAT32_MAX, D3D12_FLOAT32_MAX, D3D12_FLOAT32_MAX }, Peio::Float3{ -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX, -D3D12_FLOAT32_MAX  } } };
	for (UINT i = 0; i < tree.GetNumLeaves(); i++)
		tree.GetLeaves()[i] = { (UINT)-1 };

	tree.Insert(Peio::Vxl::PositionLeaf{ 0 });
	tree.Insert(Peio::Vxl::PositionLeaf{ 1 });
	tree.Insert(Peio::Vxl::PositionLeaf{ 2 });
	tree.Insert(Peio::Vxl::PositionLeaf{ 3 });
	tree.Insert(Peio::Vxl::PositionLeaf{ 4 });
	tree.Insert(Peio::Vxl::PositionLeaf{ 5 });
	Print(tree);
	return 0;

	try {

		Peio::Gfx::Init();

		Peio::Int2 windowSize = { 640, 360 };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };

		Peio::Win::Window window;
		window.CreateClass("Peio Sandbox", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_POPUP, 0, { 100, 100 }, windowSize);
		
		window.Show();
		ShowCursor(FALSE);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		Peio::Win::RawMouseListener listener;
		listener.Register(window.GetHWND());
		Peio::Win::Input::AddListener(&listener);
		
		Handler handler;
		Peio::Win::Input::AddEventHandler(&handler);
		
		UINT numVoxels = 128;
		
		Peio::Vxl::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = { numVoxels, 0, 0.5f, 9, 3, 0.01f, 0.99f };
		
		Peio::Vxl::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.Allocate(2);
		materialBuffer.GetSubresourceBuffer()[0] = { { 0.0f, 0.5f, 0.7f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 1.0f };
		materialBuffer.GetSubresourceBuffer()[1] = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 10.0f, 0.0f, 10.0f }, 1.0f };
		
		Peio::Vxl::SubresourceBuffer<Peio::Float3> voxelPositionBuffer;
		voxelPositionBuffer.Allocate(numVoxels);
		
		{
			float rad = 5.0f;
			for (UINT i = 0; i < numVoxels; i++) {
				float radius = (float)i / (float)(numVoxels - 1) * rad;
				float y = rad - (float)i / (float)(numVoxels - 1) * rad;
				float x = cos(GOLDEN_ANGLE * i) * radius;
				float z = sin(GOLDEN_ANGLE * i) * radius;
				voxelPositionBuffer.GetSubresourceBuffer()[i] = { x, y, z };
			}
		}
		voxelPositionBuffer.GetSubresourceBuffer()[0] = { 0.0f, 0.0f, 1.0f };
		voxelPositionBuffer.GetSubresourceBuffer()[1] = { 1.5f, 0.0f, 1.0f };
		voxelPositionBuffer.GetSubresourceBuffer()[2] = { 0.0f, 2.0f, 1.0f };
		
		Peio::Vxl::SubresourceBuffer<UINT> voxelMaterialBuffer;
		voxelMaterialBuffer.Allocate(numVoxels);
		for (UINT i = 0; i < numVoxels; i++)
		voxelMaterialBuffer.GetSubresourceBuffer()[i] = { 0 };

		voxelMaterialBuffer.GetSubresourceBuffer()[0] = { 1 };

		Peio::Gfx::ShaderResourceView srv;
		srv.InitBuffer(
			{ sizeof(Peio::Vxl::VoxelScene), sizeof(Material) * 2, sizeof(Peio::Float3) * numVoxels, sizeof(UINT) * numVoxels },
			{ 1, 1, numVoxels, numVoxels },
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
		double frameLength = 1.0f / 300.0;
		
		Peio::Clock<double> fpsClock;
		int frameCount = 0;
		
		Peio::Clock<double> deltaClock;
		
		float acceleration = 1.0f;
		float retardation = 0.5f;
		
		Peio::Gfx::MultiPassGraphics mpGraphics;
		mpGraphics.Init(DXGI_FORMAT_R32G32B32A32_FLOAT, windowSize, 3, 2);
		
		Peio::Vxl::DenoiseRenderer denoiser;
		denoiser.Init(graphics.GetCommandList(), (Peio::Float2)windowSize);

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
		
			if (Keydown(VK_RIGHT)) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].x() += 0.1f;
			}
			if (Keydown(VK_LEFT)) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].x() -= 0.1f;
			}
			if (Keydown(VK_UP)) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].z() += 0.1f;
			}
			if (Keydown(VK_DOWN)) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].z() -= 0.1f;
			}
			if (Keydown('O')) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].y() += 0.1f;
			}
			if (Keydown('L')) {
				voxelPositionBuffer.GetSubresourceBuffer()[0].y() -= 0.1f;
			}
			srv.GetResources()[2].Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());
		
			camera.position += camera.velocity;
		
			camera.velocity -= camera.velocity * retardation;
			//camera.velocity -= camera.velocity * (retardation / (float)deltaTime);
		
			renderer.SetCameraPosition(camera.position);
			renderer.SetCameraRotation(camera.rotation);
			renderer.UpdateCamera(graphics.GetCommandList());
		
			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
			
			mpGraphics.BeginPass(graphics.GetCommandList(), graphics.GetFrameIndex(), { 1.0f, 0.0f, 0.0f, 1.0f });
			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect);
			mpGraphics.EndPass(graphics.GetCommandList());
			
			graphics.GetRenderTargets().SetRenderTarget(graphics.GetCommandList());
			denoiser.Render(graphics.GetCommandList(), viewPort, scissorRect, mpGraphics.GetPreviousSrv().GetHeap(), mpGraphics.GetPreviousGpuHandle());
		
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