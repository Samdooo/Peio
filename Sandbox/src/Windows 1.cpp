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

int main() {

	Peio::Gfx::Init();

	Peio::Med::Encoder encoder;
	encoder.Init(AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, { 1920, 1080 }, 60, 10'000'000);

	
	Peio::Gfx::MediaGraphics graphics;
	graphics.Init({ 1920, 1080 }, 2);
	graphics.Clear({ 0.0f, 0.0f, 1.0f, 1.0f });
	graphics.Render();

	std::ofstream ofile("test.h264", std::ios::binary);
	
	Peio::Med::Frame frame;
	for (int i = 0; i < 240; i++) {
		graphics.Clear({ (float)i / 240.0f, 0.0f, 1.0f, 1.0f});
		graphics.Render();
		frame.Init(graphics.GetBuffer(), { 1920, 1080 }, AV_PIX_FMT_RGBA);
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