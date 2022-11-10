#include <Peio/Windows/Window.h>
#include <Peio/Windows/Input.h>
#include <Peio/Windows/KeyboardListener.h>
#include <iostream>

int main() {

	Peio::Win::Window window;
	window.CreateClass("Peio Sandbox Class", 0);
	window.RegisterClass();
	window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { 100, 100 }, { 1280, 720 });
	window.Show();

	Peio::Win::Input::AddListener(&Peio::Win::keyboardListener);
	//
	//Peio::Procedure<Peio::Win::WinEvent*> proc = Peio::Procedure<Peio::Win::KeyboardEvent*>([](Peio::Win::KeyboardEvent* e) {
	//	std::cout << "Keyboard " << e->key << std::endl;
	//});
	//Peio::Win::Input::AddListener(&proc);

	while (true) {
		window.HandleMessages();
	}

	return 0;
}