export module Peio.Graphics.QueueFamilies;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.VulkanException;
import <vector>;
import Peio.Graphics.PhysicalDevice;
import Peio.Essentials.Types;

export namespace Peio {

	namespace Graphics {

		struct FamilyNotFoundException : public Exception {
			FamilyNotFoundException() : Exception("All necessary queue families were not found.") {}
		};

		class QueueFamilies {

			std::vector<vk::QueueFamilyProperties> families{};

			uint graphicsInd = -1;

		public:

			PhysicalDevice& physicalDevice;
			QueueFamilies(PhysicalDevice& physicalDevice) : physicalDevice(physicalDevice) {}

			void Init();

			uint GetGraphicsIndex() const;

		};

	}
	using namespace Graphics;

}