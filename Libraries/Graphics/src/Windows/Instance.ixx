export module Peio.Graphics.Windows.Instance;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.Windows.PhysicalDevice;
export import Peio.Essentials.Exception;

export namespace Peio {

	namespace Graphics {

		namespace Windows {

			namespace InstanceExtensions {

				const std::vector<const char*> windowsGraphics = {
					vk::KHRSurfaceExtensionName,
					vk::KHRWin32SurfaceExtensionName
				};

			}

			struct NoSuitablePhysicalDeviceException : public Exception {
				NoSuitablePhysicalDeviceException() : Exception("No suitable physical device for windows graphics was found.") {}
			};

			class Instance : public Graphics::Instance {

				 Windows::PhysicalDevice physicalDevice;

			public:

				void Init(const std::string& appName);

			};

		}
		using namespace Windows;

	}
	using namespace Graphics;

}