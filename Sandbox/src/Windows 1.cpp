#include <Peio/Windows/Window.h>
#include <Peio/Windows/Input.h>
#include <Peio/Windows/RawKeyboardListener.h>
#include <Peio/Windows/RawMouseListener.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <Peio/Graphics/MediaGraphics.h>
#include <Peio/Media/Encoder.h>
#include <Peio/Media/Remux.h>
#include <iostream>
#include <Peio/Buffer.h>
#include <fstream>
#include <Peio/Graphics/Shader.h>
#include <filesystem>
#include <Peio/Graphics/Renderer.h>
#include <Peio/Files.h>

struct Vertex {
	Peio::Float2 pos = {};
};

int main() {

	Peio::Gfx::Init();

	Peio::Med::Encoder encoder;
	encoder.Init(AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, { 1280, 720 }, 60, 10'000'000);	
	
	Peio::Gfx::MediaGraphics graphics;
	graphics.Init({ 1280, 720 }, 2);
	
	std::ofstream ofile("test.h264", std::ios::binary);
	
	Peio::Gfx::Renderer renderer;
	//renderer.vs.Compile("../Sandbox/src/WinVS.hlsl", "vs_6_0");
	//std::cout << renderer.vs.data.size() << std::endl;
	//renderer.ps.Compile("../Sandbox/src/WinPS.hlsl", "ps_6_0");
	//std::cout << renderer.ps.data.size() << std::endl;
	char* vsData = nullptr;
	size_t vsSize = Peio::Files::ReadBinary("../Sandbox/WinVS.cso", &vsData);
	renderer.vs.data = std::string(vsData, vsSize);
	char* psData = nullptr;
	size_t psSize = Peio::Files::ReadBinary("../Sandbox/WinPS.cso", &psData);
	renderer.ps.data = std::string(psData, psSize);


	renderer.vertexLayout.elements = {
		Peio::Gfx::VertexElement{"POSITION", DXGI_FORMAT_R32G32_FLOAT}
	};
	renderer.Create();
	
	Peio::Buffer<Vertex> buf;
	buf.GetVector().resize(3);
	buf[0] = { {0.5f, 0.5f } };
	buf[1] = { {-0.5f, 0.5f } };
	buf[2] = { {0.0f, -0.5f } };

	Peio::Gfx::VertexBuffer vb;
	vb.Init(sizeof(Vertex), buf.GetSize(), false);
	vb.buffer.SetData((byte*)buf.GetData(), sizeof(Vertex) * buf.GetSize());
	vb.Upload(graphics.GetCommandList());

	Peio::Med::Frame frame;
	for (int i = 0; i < 240; i++) {
		graphics.Clear({ (float)i / 240.0f, 0.0f, 1.0f, 1.0f});

		renderer.Render(graphics.GetCommandList(), &vb, { 0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f }, { 0, 0, 1280, 720 });

		graphics.Render();
	
		frame.Init(graphics.GetBuffer(), { 1280, 720 }, AV_PIX_FMT_RGBA);
		encoder.EncodeFrame(&frame, [&ofile](const byte* data, int size) {
			ofile.write((const char*)data, size);
			});
	}
	ofile.close();
	
	Peio::Med::Remux("test.h264", "test.mp4");

	//Peio::Win::Window window;
	//window.CreateClass("Peio Sandbox Class", 0);
	//window.RegisterClass();
	//window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { 100, 100 }, { 1280, 720 });
	//window.Show();
	//
	//Peio::Win::RawKeyboardListener::Register(window.GetHWND());
	//Peio::Win::Input::listeners.Insert(&Peio::Win::rawKeyboardListener);
	//
	//Peio::Procedure<Peio::Win::RawKeyDownEvent*> keyListener = [](Peio::Win::RawKeyDownEvent* e) {
	//	std::cout << e->key << std::endl;
	//};
	//Peio::Win::Input::listeners.Insert(&keyListener);
	//
	//Peio::Win::RawMouseListener::Register(window.GetHWND());
	//Peio::Win::Input::listeners.Insert(&Peio::Win::rawMouseListener);
	//
	//Peio::Win::Input::listeners.Add((Peio::Procedure<Peio::Win::RawMouseButtonDownEvent*>)
	//	[](Peio::Win::RawMouseButtonDownEvent* e) {
	//		std::cout << "Button " << (int)e->button << " down" << std::endl;
	//	}
	//);
	//
	//Peio::Gfx::WinGraphics graphics;
	//graphics.Init(window.GetHWND(), { 1280, 720 }, 2, false);
	//
	//Peio::Buffer<char> buf;
	//buf.GetVector() = { 'a', 'b' };
	//
	//while (true) {
	//	window.HandleMessages();
	//
	//	graphics.Clear({ 0.0f, 0.0f, 1.0f, 1.0f });
	//
	//	graphics.Render();
	//}

	return 0;
}