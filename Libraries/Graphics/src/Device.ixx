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

			Instance& instance;
			PhysicalDevice& physicalDevice;

			Device(Instance& instance, PhysicalDevice& physicalDevice)
				: instance(instance), physicalDevice(physicalDevice) {}

			void Init();
			void Destroy();

			vk::Device& Get();

		};

	}
	using namespace Graphics;

}