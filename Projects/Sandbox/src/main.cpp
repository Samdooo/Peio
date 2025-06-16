
#include <iostream>

import Peio.Essentials.Types;
import Peio.Essentials.Clock;
import Peio.Essentials.Vec;

import Peio.Windows.Window;

using namespace Peio;

int main() {

	Windows::CreateClass("Peio Sandbox Class");

	Window window{};
	Int2 windowSize = { 1280, 720 };
	window.Create("Peio Sandbox Class", "Peio Sandbox", windowSize);
	window.Show();

	while (window.IsOpen()) {
		window.HandleMessages();
	}

	return 0;
}