export module Peio.Graphics.Instance;

import <vulkan/vulkan.hpp>;
export import Peio.Graphics.VulkanException;

import <vector>;
import <string>;

export namespace Peio {

	namespace Graphics {

		struct FailedAvailableValidationLayerRetrievalException : public VulkanException {
			FailedAvailableValidationLayerRetrievalException(vk::Result vkResult) : VulkanException("Failed to get available validation layers", vkResult) {}
		};
		std::vector<std::string> GetAvailableValidationLayers();

		struct FailedValidationEnablingException : public VulkanException {
			FailedValidationEnablingException(vk::Result vkResult) : VulkanException("Failed to enable Vulkan validation layers", vkResult) {}
		};
		struct ValidationLayerNotAvailableException : public Exception {
			ValidationLayerNotAvailableException() : Exception("All requested validation layers are not available") {}
		};
		struct FailedInstanceCreationException : public VulkanException {
			FailedInstanceCreationException(vk::Result vkResult) : VulkanException("Failed to create Vulkan instance", vkResult) {}
		};

		class Instance {

			vk::Instance instance{};

		#ifdef _DEBUG
			const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};
		#else
			const std::vector<const char*> validationLayers{};
		#endif

		public:

			void Init(std::string appName);

			vk::Instance& Get();
			const std::vector<const char*>& GetValidationLayers() const;

			void Destroy();

		};

	}
	using namespace Graphics;

}