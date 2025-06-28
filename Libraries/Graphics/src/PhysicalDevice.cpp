module Peio.Graphics.PhysicalDevice;

import Peio.Essentials.Types;

const vk::QueueFamilyProperties& Peio::Graphics::QueueFamily::GetProperties() const
{
	return properties;
}

uint Peio::Graphics::QueueFamily::GetIndex() const
{
	return index;
}

void Peio::Graphics::PhysicalDevice::Init(vk::PhysicalDevice physicalDevice)
{
	this->physicalDevice = physicalDevice;
}

vk::PhysicalDeviceProperties Peio::Graphics::PhysicalDevice::GetProperties() const
{
	return physicalDevice.getProperties();
}

vk::PhysicalDevice& Peio::Graphics::PhysicalDevice::Get()
{
	return physicalDevice;
}

std::vector<Peio::Graphics::QueueFamily> Peio::Graphics::PhysicalDevice::GetQueueFamilies() const
{
	uint familyCount;
	physicalDevice.getQueueFamilyProperties(&familyCount, nullptr);
	
	std::vector<vk::QueueFamilyProperties> vkFamilies(familyCount);
	physicalDevice.getQueueFamilyProperties(&familyCount, &vkFamilies[0]);

	std::vector<QueueFamily> families{};
	for (uint i = 0; i < familyCount; i++)
		families.push_back(QueueFamily(vkFamilies[i], i));

	return families;
}

std::vector<Peio::Graphics::PhysicalDevice> Peio::Graphics::Instances::PhysicalDeviceEnum::GetPhysicalDevices() const
{
	uint deviceCount;
	vk::Result result = instance.Get().enumeratePhysicalDevices(&deviceCount, nullptr);
	if (result != vk::Result::eSuccess) {
		throw FailedDeviceEnumerationException(result);
	}

	std::vector<vk::PhysicalDevice> vkDevices(deviceCount);
	result = instance.Get().enumeratePhysicalDevices(&deviceCount, &vkDevices[0]);
	if (result != vk::Result::eSuccess) {
		throw FailedDeviceEnumerationException(result);
	}
	std::vector<PhysicalDevice> devices{};
	for (uint i = 0; i < deviceCount; i++) {
		devices.push_back(PhysicalDevice{});
		devices.back().Init(vkDevices[i]);
	}
	return devices;
}