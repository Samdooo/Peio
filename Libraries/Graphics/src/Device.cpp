module Peio.Graphics.Device;

void Peio::Graphics::Device::Init()
{
	vk::DeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.queueFamilyIndex = queueFamilies.GetGraphicsIndex();
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	vk::PhysicalDeviceFeatures deviceFeautures{};

	vk::DeviceCreateInfo createInfo{};
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeautures;

	createInfo.enabledExtensionCount = 0;

	createInfo.enabledLayerCount = (uint)instance.GetValidationLayers().size();
	if (createInfo.enabledLayerCount > 0) {
		createInfo.ppEnabledLayerNames = &instance.GetValidationLayers()[0];
	}

	vk::Result result = physicalDevice.Get().createDevice(&createInfo, nullptr, &device);
	if (result != vk::Result::eSuccess) {
		throw FailedDeviceCreationException(result);
	}
}

void Peio::Graphics::Device::Destroy()
{
	device.destroy(nullptr);
}

vk::Device& Peio::Graphics::Device::Get()
{
	return device;
}