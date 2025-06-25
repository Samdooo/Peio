export module Peio.Graphics.PhysicalDevice;

export import Peio.Graphics.VulkanException;
import Peio.Graphics.Instance;

export namespace Peio {

	namespace Graphics {

		struct FailedDeviceEnumerationException : public VulkanException {
			FailedDeviceEnumerationException(vk::Result vkResult) : VulkanException("Failed to enumerate physical devices", vkResult) {}
		};
		struct NoAvailableDevicesException : public Exception {
			NoAvailableDevicesException() : Exception("There are no available physical devices with Vulkan support") {}
		};

		class PhysicalDevice {

			vk::PhysicalDevice physicalDevice;

		public:

			Instance& instance;
			PhysicalDevice(Instance& instance) : instance(instance) {}

			void Init();
			vk::PhysicalDevice& Get();

		};

	}
	using namespace Graphics;

}