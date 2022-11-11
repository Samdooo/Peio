#include <Peio/Windows/Window.h>
#include <Peio/Windows/Input.h>
#include <Peio/Windows/RawKeyboardListener.h>
#include <iostream>

int main() {

	Peio::Win::Window window;
	window.CreateClass("Peio Sandbox Class", 0);
	window.RegisterClass();
	window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { 100, 100 }, { 1280, 720 });
	window.Show();

	Peio::Win::RawKeyboardListener::Register(window.GetHWND());
	Peio::Win::Input::AddListener(&Peio::Win::rawKeyboardListener);

	Peio::Procedure<Peio::Win::RawKeyDownEvent*> proc = [](Peio::Win::RawKeyDownEvent* e) {
		std::cout << e->key << std::endl;
	};
	Peio::Win::Input::AddListener(&proc);

	while (true) {
		window.HandleMessages();
	}

	return 0;
}