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

#include <unordered_map>

#define PI 3.14159265358979f
#define PHI 0.618033988749895f

struct InputVertex {
	Peio::Float2 position = {};
	Peio::Float3 cameraPosition = {};
	Peio::Float2 rotation = {};
	float fov = 1.5f;
	float aspectRatio = 1.0f;
};

struct Camera {
	Peio::Float3 position = {};
	Peio::Float2 rotation = {};
	float fov = 0.0f;
	Peio::Float3 velocity = {};
};

using uint = unsigned int;

struct MaterialTree {

	struct Group {
		uint indices[2][2][2] = {
			{ { (uint)-1, (uint)-1 }, { (uint)-1, (uint)-1 } },
			{ { (uint)-1, (uint)-1 }, { (uint)-1, (uint)-1 } }
		};
	};

	std::vector<Group> groups = {};
	//std::vector<uint> references = {};
	uint numLayers = 0;
	uint curFree = 1;

	void Init(size_t maxSize, size_t numLayers) {
		groups.resize(maxSize, Group());
		this->numLayers = numLayers;
	}

	_NODISCARD uint* GetMaterial(uint x, uint y, uint z, bool insert = false) {
		Group* cur = &groups[0];
		for (uint l = 0; l < numLayers; l++) {
			uint mask = 1U << (numLayers - l - 1);
			uint& index = cur->indices[(bool)(x & mask)][(bool)(y & mask)][(bool)(z & mask)];
			if (l == numLayers - 1) {
				return &index;
			}
			if (index == -1) {
				if (insert) {
					index = curFree;
					curFree++;
				}
				else {
					return nullptr;
				}
			}
			cur = &groups[index];
		}
	}

};

bool Keydown(int key) {
	return GetKeyState(key) & 0x8000;
}

int main() {

	try {

		Peio::Gfx::Init();

		Peio::Int2 windowSize = { 640, 360 };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize.x(), (float)windowSize.y(), 0.0f, 1.0f };
		RECT scissorRect = { 0, 0, windowSize.x(), windowSize.y() };

		Peio::Win::Window window;
		window.CreateClass("Peio Window Class", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, windowSize);

		Peio::Gfx::WinGraphics graphics;
		graphics.Init(window.GetHWND(), windowSize, 3, false);
		
		Camera camera;
		camera.fov = 1.5f;

		Peio::Gfx::VertexBuffer<InputVertex> vertexBuffer = {};
		vertexBuffer.Allocate(6);
		for (UINT i = 0; i < 6; i++) {
			vertexBuffer.GetSubresourceBuffer()[i] = InputVertex{
				{ (i >= 1 && i <= 3) ? 1.0f : -1.0f, (i >= 2 && i <= 4) ? -1.0f : 1.0f }, camera.position, camera.rotation, camera.fov, (float)windowSize.y() / (float)windowSize.x()
			};
		}
		vertexBuffer.Upload(graphics.GetCommandList());

		Peio::Gfx::PipelineState pipelineState = {};

		UINT numSrvs = 1;
		UINT numUavs = 0;
		std::vector<D3D12_ROOT_PARAMETER> rootParams(numSrvs + numUavs);
		for (UINT i = 0; i < numSrvs; i++)
			rootParams[i] = Peio::Gfx::RootParameter::CreateShaderResourceView(i, D3D12_SHADER_VISIBILITY_PIXEL);
		for (UINT i = 0; i < numUavs; i++)
			rootParams[numSrvs + i] = Peio::Gfx::RootParameter::CreateUnorderedAccessView(i + 1, D3D12_SHADER_VISIBILITY_PIXEL);

		ID3D12RootSignature* rootSignature = Peio::Gfx::RootSignature::Create(rootParams, {},
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		pipelineState.Init(
			Peio::Gfx::InputLayout::Create({
				Peio::Gfx::InputElement::Create("POSITION", DXGI_FORMAT_R32G32_FLOAT),
				Peio::Gfx::InputElement::Create("CAMERA_POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
				Peio::Gfx::InputElement::Create("ROTATION", DXGI_FORMAT_R32G32_FLOAT),
				Peio::Gfx::InputElement::Create("FOV", DXGI_FORMAT_R32_FLOAT),
				Peio::Gfx::InputElement::Create("ASPECT_RATIO", DXGI_FORMAT_R32_FLOAT),
				}), rootSignature, Peio::Gfx::Shader::Load("TestVS.cso"),
				Peio::Gfx::Shader::Load("TestPS.cso"), false
				);

		MaterialTree tree;
		tree.Init((1 << 24), 24);
		uint rad = 100;
		for (uint x = 0; x < rad; x++) {
			for (uint y = 0; y < rad; y++) {
				for (uint z = 0; z < rad; z++) {
					*tree.GetMaterial(x * 2, y * 2, z * 2, true) = (x * rad * rad) + (y * rad) + z;
				}
			}
		}
		//*tree.GetMaterial(15, 0, 0, true) = 0;
		//*tree.GetMaterial(0, 0, 3, true) = 0;
		//*tree.GetMaterial(3, 2, 9, true) = 0;

		Peio::Gfx::BufferSRV treeSrv;
		treeSrv.Init(sizeof(MaterialTree::Group) * tree.groups.size(), tree.groups.size(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		Peio::Gfx::SubresourceBuffer<MaterialTree::Group> treeBuffer;
		treeBuffer.SetBuffer(&tree.groups[0], tree.groups.size());
		treeSrv.Upload(treeBuffer.GetResourceData(), graphics.GetCommandList());

		float acceleration = 1.0f;
		float retardation = 0.5f;

		Peio::FunctionHandler<Peio::Win::RawMouseMoveEvent> mouseMoveHandler(
			[&camera](Peio::Win::RawMouseMoveEvent* event) -> bool {
				camera.rotation.x() -= (float)event->movement.x() / 500.0f;
				camera.rotation.y() -= (float)event->movement.y() / 500.0f;
				return false;
			}
		);
		Peio::Win::RawMouseListener rawMouseListener;
		Peio::Win::RawMouseListener::Register(window.GetHWND());
		Peio::Win::Input::AddListener(&rawMouseListener);

		Peio::Win::Input::eventHandlers.Insert(&mouseMoveHandler, mouseMoveHandler.GetBaseHandler<Peio::Win::WinEvent>());

		Peio::Clock<double> deltaClock;

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

			camera.position += camera.velocity;
			camera.velocity -= camera.velocity * retardation;

			for (UINT i = 0; i < 6; i++) {
				vertexBuffer.GetSubresourceBuffer()[i].cameraPosition = camera.position;
				vertexBuffer.GetSubresourceBuffer()[i].rotation = camera.rotation;
			}
			vertexBuffer.Upload(graphics.GetCommandList());

			graphics.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

			pipelineState.Set(graphics.GetCommandList());
			graphics.GetCommandList()->RSSetViewports(1, &viewPort);
			graphics.GetCommandList()->RSSetScissorRects(1, &scissorRect);

			graphics.GetCommandList()->SetGraphicsRootShaderResourceView(0, treeSrv.GetGPUAddress());

			graphics.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			graphics.GetCommandList()->IASetVertexBuffers(0, 1, &vertexBuffer.GetBufferView());
			graphics.GetCommandList()->DrawInstanced(6, 1, 0, 0);

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