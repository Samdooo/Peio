export module Peio.Graphics.Device;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.VulkanException;
import Peio.Graphics.Instance;
import Peio.Graphics.PhysicalDevice;
import Peio.Graphics.QueueFamilies;

export namespace Peio {

	namespace Graphics {

		struct FailedDeviceCreationException : public VulkanException {
			FailedDeviceCreationException(vk::Result result) : VulkanException("Failed to create logical device.", result) {}
		};

		class Device {

			vk::Device device{};

		public:

			Instance& instance;
			PhysicalDevice& physicalDevice;
			QueueFamilies& queueFamilies;

			Device(QueueFamilies& queueFamilies)
				: instance(queueFamilies.physicalDevice.instance)
				, physicalDevice(queueFamilies.physicalDevice)
				, queueFamilies(queueFamilies) {}

			void Init();
			void Destroy();

			vk::Device& Get();

		};

	}
	using namespace Graphics;

}