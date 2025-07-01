export module Peio.Graphics.Windows.Instance;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.Windows.PhysicalDevice;
export import Peio.Essentials.Exception;

export namespace Peio {

	namespace Graphics {

		namespace InstanceExtensions {

			const std::vector<const char*> windowsGraphics = {
				vk::KHRSurfaceExtensionName,
				vk::KHRWin32SurfaceExtensionName
			};

		}

		struct NoSuitablePhysicalDeviceException : public Exception {
			NoSuitablePhysicalDeviceException() : Exception("No suitable physical device for windows graphics was found.") {}
		};

		namespace Instances {

			class Windows {

				WindowsPhysicalDevice physicalDevice;

			public:

				Instance& instance;
				Windows(Instance& instance) : instance(instance) {}

				void Init(const std::string& appName);

			};

		}

		class WindowsInstance : public Instance, public Instances::Windows {

		public:

			WindowsInstance() : Windows((Instance&)*this) {}

			using Windows::Init;

		};


	}
	using namespace Graphics;

}