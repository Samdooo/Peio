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
		if (!Keydown(VK_ESCAPE)) {
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
			if (j > 16)
				break;
			std::cout << "[ " << tree.GetBranches()[i][j].descriptor << " ]" << "  ";
			//std::cout << "[ " << tree.GetBranches()[i][j].boundaries[0].ToString() << " - " << tree.GetBranches()[i][j].boundaries[1].ToString() << " ] ";
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

int main() {

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
		
		UINT numVoxels = 10 * 10 * 10;
		
		Peio::Vxl::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = { numVoxels, 0, 0.5f, 9, 3, 0.01f, 0.99f };
		
		Peio::Vxl::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.Allocate(2);
		materialBuffer.GetSubresourceBuffer()[0] = { { 0.0f, 0.5f, 0.7f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 1.0f };
		materialBuffer.GetSubresourceBuffer()[1] = { { 0.0f, 0.0f, 0.0f, 1.0f }, { 10.0f, 0.0f, 10.0f }, 1.0f };
		
		Peio::Vxl::SubresourceBuffer<Peio::Float3> voxelPositionBuffer;
		voxelPositionBuffer.Allocate(numVoxels);
	
		//{
		//	float rad = 200.0f;
		//	for (UINT i = 0; i < numVoxels; i++) {
		//		float radius = (float)i / (float)(numVoxels - 1) * rad;
		//		float y = rad - (float)i / (float)(numVoxels - 1) * rad;
		//		float x = cos(GOLDEN_ANGLE * i) * radius;
		//		float z = sin(GOLDEN_ANGLE * i) * radius;
		//		voxelPositionBuffer.GetSubresourceBuffer()[i] = { x, y, z };
		//	}
		//}
		for (UINT x = 0; x < 10; x++) {
			for (UINT y = 0; y < 10; y++) {
				for (UINT z = 0; z < 10; z++) {
					voxelPositionBuffer.GetSubresourceBuffer()[(x * 10 * 10) + (y * 10) + z] = { (float)x * 2, (float)y * 2, (float)z * 2 };
				}
			}
		}

		//voxelPositionBuffer.GetSubresourceBuffer()[0] = { 0.0f, 0.0f, 1.0f };
		//voxelPositionBuffer.GetSubresourceBuffer()[1] = { 1.5f, 0.0f, 1.0f };
		//voxelPositionBuffer.GetSubresourceBuffer()[2] = { 0.0f, 2.0f, 1.0f };
		//voxelPositionBuffer.GetSubresourceBuffer()[3] = { 4.0f, 2.0f, -1.0f };
		//voxelPositionBuffer.GetSubresourceBuffer()[4] = { 5.5f, 2.0f, -1.0f };
		//voxelPositionBuffer.GetSubresourceBuffer()[5] = { 4.0f, 4.0f, -1.0f };
	
		Peio::Vxl::SubresourceBuffer<UINT> voxelMaterialBuffer;
		voxelMaterialBuffer.Allocate(numVoxels);
		for (UINT i = 0; i < numVoxels; i++)
			voxelMaterialBuffer.GetSubresourceBuffer()[i] = { 0 };
		voxelMaterialBuffer.GetSubresourceBuffer()[0] = { 1 };
	
		PositionTree tree;
		tree.Allocate(14, 3);
		ZeroMemory(tree.GetBranches()[0], tree.GetNumBranches() * sizeof(Peio::Vxl::PositionBranch));
		ZeroMemory(tree.GetLeaves(), tree.GetNumLeaves() * sizeof(Peio::Vxl::PositionLeaf));
		tree.voxelPositions = voxelPositionBuffer.GetSubresourceBuffer();
		for (UINT i = 0; i < numVoxels; i++) {
			tree.Insert(Peio::Vxl::PositionLeaf{ i });
		}
	
		Peio::Vxl::SubresourceBuffer<Peio::Vxl::PositionBranch> branchBuffer;
		branchBuffer.SetBuffer(tree.GetBranches()[0], tree.GetNumBranches());
		
		Peio::Vxl::SubresourceBuffer<Peio::Vxl::PositionLeaf> leafBuffer;
		leafBuffer.SetBuffer(tree.GetLeaves(), tree.GetNumLeaves());
	
		Peio::Gfx::ShaderResourceView srv;
		srv.InitBuffer(
			{ sizeof(Peio::Vxl::VoxelScene), sizeof(Material) * 2, sizeof(Peio::Float3) * numVoxels, sizeof(UINT) * numVoxels, 
			  sizeof(Peio::Vxl::PositionBranch) * tree.GetNumBranches(), sizeof(Peio::Vxl::PositionLeaf) * tree.GetNumLeaves() },
			{ 1, 1, numVoxels, numVoxels, (UINT)tree.GetNumBranches(), (UINT)tree.GetNumLeaves() },
			{ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE }
		);
		srv.GetResources()[0].Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[1].Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[2].Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[3].Upload(voxelMaterialBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[4].Upload(branchBuffer.GetResourceData(), graphics.GetCommandList());
		srv.GetResources()[5].Upload(leafBuffer.GetResourceData(), graphics.GetCommandList());
	
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
	
		while (true) {
			window.HandleMessages();
			double deltaTime = deltaClock.Restart().Seconds();
		
			if (Keydown(VK_CONTROL)) {
				acceleration = 10.0f;
			}
			else {
				acceleration = 1.0f;
			}

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

			if (Keydown('U')) {
				//while (Keydown('U'));

				Peio::Float3 ray(0, 0, 1);
				ray = RotateX(ray, camera.rotation.y());
				ray = RotateY(ray, camera.rotation.x());
				PositionTree::Ray result = tree.TraceRay(camera.position, ray, -1);
				if (result.side != -1) {
					tree.Remove(result.it);
					srv.GetResources()[4].Upload(branchBuffer.GetResourceData(), graphics.GetCommandList());
					srv.GetResources()[5].Upload(leafBuffer.GetResourceData(), graphics.GetCommandList());
				}
			}
	
			//tree.UpdateBoundaries(tree.GetLeafIterator().GetParent());
			//
			//srv.GetResources()[2].Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());
			//srv.GetResources()[4].Upload(branchBuffer.GetResourceData(), graphics.GetCommandList());
			//srv.GetResources()[5].Upload(leafBuffer.GetResourceData(), graphics.GetCommandList());
	
			camera.position += camera.velocity;
		
			camera.velocity -= camera.velocity * retardation;
		
			renderer.SetCameraPosition(camera.position);
			renderer.SetCameraRotation(camera.rotation);
			renderer.UpdateCamera(graphics.GetCommandList());
		
			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect);
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