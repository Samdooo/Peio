export module Peio.Graphics.VulkanException;

import <vulkan/vulkan.hpp>;
export import Peio.Essentials.Exception;

export namespace Peio {

	namespace Graphics {

		struct VulkanException : public Exception {
		
			const vk::Result vkResult;

			VulkanException(const std::string& message, vk::Result vkResult) : Exception(message), vkResult(vkResult) {}
		
		};

	}
	using namespace Graphics;

}