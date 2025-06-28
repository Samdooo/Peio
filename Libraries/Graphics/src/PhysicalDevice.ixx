export module Peio.Graphics.PhysicalDevice;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.Instance;
export import Peio.Essentials.Types;

export namespace Peio {

	namespace Graphics {

		struct FailedQueueFamilyEnumerationException : public VulkanException {
			FailedQueueFamilyEnumerationException(vk::Result vkResult) : VulkanException("Failed to enumerate queue families", vkResult) {}
		};

		class QueueFamily {

			vk::QueueFamilyProperties properties;
			uint index;

		public:

			QueueFamily(vk::QueueFamilyProperties properties, uint index) : properties(properties), index(index) {}

			const vk::QueueFamilyProperties& GetProperties() const;
			uint GetIndex() const;

		};

		struct FailedDeviceEnumerationException : public VulkanException {
			FailedDeviceEnumerationException(vk::Result vkResult) : VulkanException("Failed to enumerate physical devices", vkResult) {}
		};

		class PhysicalDevice {

			vk::PhysicalDevice physicalDevice;

		public:

			void Init(vk::PhysicalDevice physicalDevice);

			vk::PhysicalDeviceProperties GetProperties() const;
			vk::PhysicalDevice& Get();

			std::vector<QueueFamily> GetQueueFamilies() const;

		};

		namespace Instances {

			class PhysicalDeviceEnum {

			public:

				Instance& instance;
				PhysicalDeviceEnum(Instance& instance) : instance(instance) {}

				std::vector<PhysicalDevice> GetPhysicalDevices() const;

			};

		}

	}
	using namespace Graphics;

}