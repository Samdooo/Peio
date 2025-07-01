export module Peio.Graphics.Windows.PhysicalDevice;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.PhysicalDevice;
export import Peio.Essentials.Types;

export namespace Peio {

	namespace Graphics {

		namespace PhysicalDevices {

			class Windows {

				struct FamilyIndices {
					uint graphics;
				} familyIndices{};

				bool suitable = true;

			public:

				PhysicalDevice& physicalDevice;
				Windows(PhysicalDevice& physicalDevice) : physicalDevice(physicalDevice) {}

				void Init(vk::PhysicalDevice physicalDevice);

				bool IsSuitable() const;

			};

		}

		class WindowsPhysicalDevice : public PhysicalDevice, public PhysicalDevices::Windows {

		public:

			WindowsPhysicalDevice() : Windows((PhysicalDevice&)*this) {}

			using Windows::Init;

		};

	}
	using namespace Graphics;

}