export module Peio.Graphics.Device;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.VulkanException;
export import Peio.Graphics.Instance;
export import Peio.Graphics.PhysicalDevice;

export namespace Peio {

	namespace Graphics {

		struct FailedDeviceCreationException : public VulkanException {
			FailedDeviceCreationException(vk::Result result) : VulkanException("Failed to create logical device", result) {}
		};

		class Device {

			vk::Device device{};

		public:



		};

	}
	using namespace Graphics;

}