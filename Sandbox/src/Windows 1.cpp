#include <Peio/Windows/Window.h>
#include <Peio/Windows/Input.h>
#include <Peio/Windows/RawKeyboardListener.h>
#include <Peio/Windows/RawMouseListener.h>
#include <Peio/Windows/KeyboardListener.h>
#include <Peio/Graphics/WinGraphics.h>
#include <iostream>

int main() {

	Peio::Gfx::Init();

	Peio::Win::Window window;
	window.CreateClass("Peio Sandbox Class", 0);
	window.RegisterClass();
	window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { 100, 100 }, { 1280, 720 });
	window.Show();

	Peio::Win::Window window2;
	window2.CreateClass("Peio Sandbox Class2", 0);
	window2.RegisterClass();
	window2.CreateWindow("Peio Sandbox2", WS_OVERLAPPEDWINDOW, 0, { 200, 200 }, { 1280, 720 });
	window2.Show();

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

	Peio::Win::Input::listeners.Insert(&Peio::Win::keyboardListener);

	Peio::Procedure<Peio::Win::KeydownEvent*> keyProc = [](Peio::Win::KeydownEvent* e) {
		std::cout << "Key " << e->key << " pressed" << std::endl;
	};
	window2.listeners.Insert(&keyProc);

	Peio::Gfx::WinGraphics graphics;
	graphics.Init(window.GetHWND(), { 1280, 720 }, 2, false);

	Peio::Gfx::WinGraphics graphics2;
	graphics2.Init(window2.GetHWND(), { 1280, 720 }, 2, false);

	while (true) {
		window.HandleMessages();
		window2.HandleMessages();

		graphics.Clear({ 0.0f, 0.0f, 1.0f, 1.0f });

		graphics.Render();

		graphics2.Clear({ 0.0f, 1.0f, 0.0f, 1.0f });

		graphics2.Render();
	}

	return 0;
}