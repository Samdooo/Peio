#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>
#include <Peio/Windows/Window.h>
#include <Peio/Windows/KeyboardListener.h>

#include <unordered_map>

struct Handler : public Peio::EventHandler<Peio::Win::KeydownEvent, Peio::Win::KeyupEvent> {

	void Handle(Peio::Win::KeydownEvent& event) {
		if (!event.prev)
			std::cout << event.key << " pressed" << std::endl;
	}

	void Handle(Peio::Win::KeyupEvent& event) {
		std::cout << event.key << " released" << std::endl;
	}

};

int main() {

	try {

		Peio::Win::Window window;
		window.CreateClass("Peio Sandbox", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Sandbox", WS_OVERLAPPEDWINDOW, 0, { CW_USEDEFAULT, CW_USEDEFAULT }, { 1280, 720 });

		window.Show();

		Peio::Win::KeyboardListener listener;
		Peio::Win::Input::AddListener(&listener);

		Handler handler;
		Peio::Win::Input::AddEventHandler<Peio::Win::KeydownEvent, Peio::Win::KeyupEvent>(&handler);

		while (true) {
			window.HandleMessages();
		}

	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}