
#include <iostream>
#include <windows.h>

import Peio.Essentials.Types;
import Peio.Essentials.Clock;
import Peio.Essentials.Vec;

import Peio.Windows.Window;
import Peio.Windows.Input;
import Peio.Windows.Keyboard;
import Peio.Windows.Mouse;

using namespace Peio;

int main() {

	try {

		Windows::CreateClass("Peio Sandbox Class");

		Window window{};
		Int2 windowSize = { 1280, 720 };
		window.Create("Peio Sandbox Class", "Peio Sandbox", windowSize);
		window.Show();

		//MessageHandler charHandler = CreateCharHandler([](CharEvent event) {
		//	std::cout << (char)event.key;
		//});
		//AddMessageHandler(&charHandler);

		RegisterRawMouseInput(window);
		
		MessageHandler rawMouseHandler = CreateRawMouseHandler(nullptr, nullptr, nullptr, [](RawMouseWheelEvent event) {
			std::cout << event.foreground << " " << event.delta << std::endl;
		});
		AddMessageHandler(&rawMouseHandler);

		RegisterRawKeyboardInput(window);

		MessageHandler rawKeyHandler = CreateRawKeyHandler([](RawKeyDownEvent event) {
			std::cout << (char)event.key << std::endl;
			}, nullptr);
		AddMessageHandler(&rawKeyHandler);

		while (window.IsOpen()) {
			window.HandleMessages();
		}

	}
	catch (Exception e) {
		std::cout << "Exception: " << e.message << std::endl;
	}

	return 0;
}