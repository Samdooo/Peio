
#include <iostream>
#include <windows.h>

import Peio.Essentials.Types;
import Peio.Essentials.Clock;
import Peio.Essentials.Vec;

import Peio.Windows.Window;
import Peio.Windows.Input;
import Peio.Windows.Keyboard;
import Peio.Windows.Mouse;

import Peio.Graphics.Windows.Instance;

using namespace Peio;

int main() {

	try {

		WindowsInstance instance{};
		instance.Init("Peio Sandbox");

		instance.Destroy();

		return 0;

		CreateClass("Peio Sandbox Class");

		Window window{};
		Int2 windowSize = { 1280, 720 };
		window.Create("Peio Sandbox Class", "Peio Sandbox", windowSize);
		window.Show();

		while (window.IsOpen()) {
			window.HandleMessages();
		}


	}
	catch (Exception e) {
		std::cout << "Exception: " << e.message << std::endl;
	}

	return 0;
}