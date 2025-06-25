export module Peio.Graphics.Init;

import Peio.Graphics.Instance;
import Peio.Graphics.PhysicalDevice;
import Peio.Graphics.QueueFamilies;
import Peio.Graphics.Device;

import Peio.Essentials.Types;
import <vector>;
import <string>;

export namespace Peio {

	namespace Graphics {

		Instance instance{};
		PhysicalDevice physicalDevice(instance);
		QueueFamilies queueFamilies(physicalDevice);
		Device device(queueFamilies);

		void Init();
		void Cleanup();

	}
	using namespace Graphics;

}