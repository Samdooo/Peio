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
#include <Peio/Graphics/UnorderedAccessView.h>
#include <functional>

#include <unordered_map>

#define PI 3.14159265358979f
#define PHI 0.618033988749895f

bool Keydown(int key) {
	return GetKeyState(key) & 0x8000;
}

struct Camera {
	Peio::Float3 position = {};
	Peio::Float2 rotation = {};
	float fov = 0.0f;
	Peio::Float3 velocity = {};
};

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

struct PrimaryRay {
	UINT collisionVoxel;
	int side; // -1 indicates no collision
	Peio::Float3 collision;
	Peio::Float3 light;
};

struct Handler : public Peio::EventHandler<Peio::Win::RawMouseMoveEvent, Peio::Win::RawMouseButtonDownEvent, Peio::Win::KeydownEvent> {

	Peio::Float2* rotation = nullptr;

	bool Handle(Peio::Win::RawMouseMoveEvent* event) override {
		if (!Keydown(VK_ESCAPE)) {
			*rotation -= (Peio::Float2)event->movement / 1000.0f;
			SetCursorPos(200, 200);
		}
		return false;
	}

	std::function<void()> destroy = nullptr;
	std::function<void()> build = nullptr;

	bool Handle(Peio::Win::RawMouseButtonDownEvent* event) override {
		if (!Keydown(VK_ESCAPE)) {
			if (event->button == Peio::Win::MouseButton::LEFT) {
				destroy();
			}
			else {
				build();
			}
		}
		return false;
	}

	std::function<void(UINT)> setMaterial = nullptr;

	bool Handle(Peio::Win::KeydownEvent* event) override {
		if (!Keydown(VK_ESCAPE) && !event->prev) {
			if (event->key >= '1' && event->key <= '9')
				setMaterial(event->key - '1');
		}
		return false;
	}

};

struct Material {

	Peio::Float4 colorEmission;
	Peio::Float3 lightEmission;

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

int main() {

	try {

		Peio::Gfx::Init();
	
		Peio::Int2 windowSize = { 1920, 1080 };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };
	
		Peio::Win::Window window;
		window.CreateClass("Peio Sandbox", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_POPUP, 0, { 0, 0 }, windowSize);
		
		window.Show();
		ShowCursor(FALSE);
	
		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);

		UINT rad = 100;

		Peio::Gfx::SubresourceBuffer<Peio::Vxl::VoxelScene> sceneBuffer;
		sceneBuffer.Allocate(1);
		sceneBuffer.GetSubresourceBuffer()[0] = {
			(Peio::Uint2)windowSize, 10, 1, 0.5f, 2
		};
		Peio::Gfx::BufferSRV sceneSrv;
		sceneSrv.Init(sceneBuffer.GetBufferSize(), sceneBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		sceneSrv.Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Material> materialBuffer;
		materialBuffer.Allocate(9);
		materialBuffer.GetSubresourceBuffer()[0] = {
			{ 0.1f, 0.7f, 0.9f, 1.0f }, { 0.0f, 0.0f, 0.0f }
		};
		materialBuffer.GetSubresourceBuffer()[1] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f }, { 9.0f, 2.0f, 8.0f }
		};
		materialBuffer.GetSubresourceBuffer()[2] = {
			{ 71.0f / 255.0f, 27.0f / 255.0f, 27.0f / 255.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }
		};
		materialBuffer.GetSubresourceBuffer()[3] = {
			{ 20.0f / 255.0f, 196.0f / 255.0f, 64.0f / 255.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }
		};
		materialBuffer.GetSubresourceBuffer()[4] = {
			{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }
		};
		materialBuffer.GetSubresourceBuffer()[5] = {
			{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }
		};
		Peio::Gfx::BufferSRV materialSrv;
		materialSrv.Init(materialBuffer.GetBufferSize(), materialBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		materialSrv.Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Peio::Float3> voxelPositionBuffer;
		voxelPositionBuffer.Allocate(rad * rad * rad + 10000);
		voxelPositionBuffer.GetSubresourceBuffer()[0] = {};
		for (UINT x = 0; x < rad; x++) {
			for (UINT y = 0; y < rad; y++) {
				for (UINT z = 0; z < rad; z++) {
					voxelPositionBuffer.GetSubresourceBuffer()[x * rad * rad + y * rad + z] = { (float)x * 1.0f, (float)y * 1.0f, (float)z * 1.0f };
				}
			}
		}
		Peio::Gfx::BufferSRV voxelPositionSrv;
		voxelPositionSrv.Init(voxelPositionBuffer.GetBufferSize(), voxelPositionBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		voxelPositionSrv.Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<UINT> voxelMaterialBuffer;
		voxelMaterialBuffer.Allocate(rad * rad * rad + 10000);
		for (UINT i = 0; i < rad * rad * rad + 1000; i++) {
			voxelMaterialBuffer.GetSubresourceBuffer()[i] = 0;
		}
		voxelMaterialBuffer.GetSubresourceBuffer()[0] = 1;
		Peio::Gfx::BufferSRV voxelMaterialSrv;
		voxelMaterialSrv.Init(voxelMaterialBuffer.GetBufferSize(), voxelMaterialBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		voxelMaterialSrv.Upload(voxelMaterialBuffer.GetResourceData(), graphics.GetCommandList());

		PositionTree positionTree;
		positionTree.Allocate(10, 4);
		positionTree.voxelPositions = voxelPositionBuffer.GetSubresourceBuffer();

		Peio::Gfx::SubresourceBuffer<Peio::Vxl::PositionBranch> positionBranchBuffer;
		positionBranchBuffer.SetBuffer(positionTree.GetBranches()[0], positionTree.GetNumBranches());
		Peio::Gfx::BufferSRV positionBranchSrv;
		positionBranchSrv.Init(positionBranchBuffer.GetBufferSize(), positionBranchBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		positionBranchSrv.Upload(positionBranchBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<Peio::Vxl::PositionLeaf> positionLeafBuffer;
		positionLeafBuffer.SetBuffer(positionTree.GetLeaves(), positionTree.GetNumLeaves());
		for (UINT i = 0; i < rad * rad * rad; i++) {
			positionTree.Insert({ i });
		}
		Peio::Gfx::BufferSRV positionLeafSrv;
		positionLeafSrv.Init(positionLeafBuffer.GetBufferSize(), positionLeafBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		positionLeafSrv.Upload(positionLeafBuffer.GetResourceData(), graphics.GetCommandList());

		Peio::Gfx::SubresourceBuffer<PrimaryRay> primaryRayBuffer;
		primaryRayBuffer.Allocate(windowSize.x() * windowSize.y());
		Peio::Gfx::BufferUAV primaryRayUav;
		primaryRayUav.Init(primaryRayBuffer.GetBufferSize(), primaryRayBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		Peio::Vxl::VoxelRenderer renderer;
		renderer.Init(graphics.GetCommandList(), 6, 1, {}, {}, PI / 2, (float)windowSize.y() / (float)windowSize.x());

		Peio::Win::RawMouseListener listener;
		listener.Register(window.GetHWND());
		Peio::Win::Input::AddListener(&listener);

		Peio::Win::KeyboardListener keyListener;
		Peio::Win::Input::AddListener(&keyListener);

		Camera camera = {};
		UINT voxelIndex = rad * rad * rad;

		Handler handler;
		handler.rotation = &camera.rotation;
		handler.build = [&camera, &positionTree, &voxelPositionBuffer, &voxelIndex]() {
			Peio::Float3 ray = { 0.0f, 0.0f, 1.0f };
			ray = RotateX(ray, camera.rotation.y());
			ray = RotateY(ray, camera.rotation.x());
			PositionTree::Ray traced = positionTree.TraceRay(camera.position, ray, -1);
			if (traced.side != -1) {
				Peio::Float3 pos = voxelPositionBuffer.GetSubresourceBuffer()[traced.collisionVoxel];
				pos[traced.side] += (traced.normal[traced.side] < 0.0f) ? -1.0f : 1.0f;
				voxelPositionBuffer.GetSubresourceBuffer()[voxelIndex] = pos;
				positionTree.Insert({ voxelIndex++ });
				//std::cout << traced.collisionVoxel << " added " << (voxelIndex - 1) << " at " << pos.ToString() << std::endl;
			}
		};
		handler.destroy = [&camera, &positionTree]() {
			Peio::Float3 ray = { 0.0f, 0.0f, 1.0f };
			ray = RotateX(ray, camera.rotation.y());
			ray = RotateY(ray, camera.rotation.x());
			PositionTree::Ray traced = positionTree.TraceRay(camera.position, ray, -1);
			if (traced.side != -1) {
				positionTree.Remove(traced.it);
			}
		};

		handler.setMaterial = [&camera, &positionTree, &voxelMaterialBuffer](UINT i) {
			Peio::Float3 ray = { 0.0f, 0.0f, 1.0f };
			ray = RotateX(ray, camera.rotation.y());
			ray = RotateY(ray, camera.rotation.x());
			PositionTree::Ray traced = positionTree.TraceRay(camera.position, ray, -1);
			if (traced.side != -1) {
				voxelMaterialBuffer.GetSubresourceBuffer()[traced.collisionVoxel] = i;
			}
		};

		Peio::Win::Input::eventHandlers.Insert(&handler, handler.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::Vxl::DenoiseRenderer denoiser;
		denoiser.Init(graphics.GetCommandList(), 0, 1, windowSize);

		Peio::Clock<double> clock;
		double frameLength = 1.0f / 60.0;
		
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

			positionTree.UpdateBoundaries(positionTree.GetLeafIterator(0).GetParent());
			positionTree.UpdateBoundaries(positionTree.GetLeafIterator(voxelIndex - 1).GetParent());
			sceneSrv.Upload(sceneBuffer.GetResourceData(), graphics.GetCommandList());
			materialSrv.Upload(materialBuffer.GetResourceData(), graphics.GetCommandList());
			voxelPositionSrv.Upload(voxelPositionBuffer.GetResourceData(), graphics.GetCommandList());
			voxelMaterialSrv.Upload(voxelMaterialBuffer.GetResourceData(), graphics.GetCommandList());
			positionBranchSrv.Upload(positionBranchBuffer.GetResourceData(), graphics.GetCommandList());
			positionLeafSrv.Upload(positionLeafBuffer.GetResourceData(), graphics.GetCommandList());

			camera.position += camera.velocity;
			
			camera.velocity -= camera.velocity * retardation;
			
			renderer.SetCameraPosition(camera.position);
			renderer.SetCameraRotation(camera.rotation);
			renderer.UpdateCamera(graphics.GetCommandList());
		
			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

			renderer.pipelineState.Set(graphics.GetCommandList());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(0, sceneSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(1, materialSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(2, voxelPositionSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(3, voxelMaterialSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(4, positionBranchSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(5, positionLeafSrv.GetGPUAddress());
			graphics.GetCommandList()->SetGraphicsRootUnorderedAccessView(6, primaryRayUav.GetGPUAddress());
			renderer.Render(graphics.GetCommandList(), viewPort, scissorRect);

			denoiser.pipelineState.Set(graphics.GetCommandList());
			graphics.GetCommandList()->SetGraphicsRootUnorderedAccessView(0, primaryRayUav.GetGPUAddress());
			denoiser.Render(graphics.GetCommandList(), viewPort, scissorRect);

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