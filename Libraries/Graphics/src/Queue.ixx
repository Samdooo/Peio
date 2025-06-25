export module Peio.GraphicsQueue;

import <vulkan/vulkan.hpp>;
import Peio.Graphics.QueueFamilies;
import Peio.Graphics.Device;

export namespace Peio {

	namespace Graphics {

		class Queue {

			vk::Queue queue{};

		public:

			QueueFamilies& queueFamilies;
			Device& device;

			Queue(Device& device) : queueFamilies(device.queueFamilies), device(device) {}

			void InitGraphics();

		};

	}
	using namespace Graphics;

}