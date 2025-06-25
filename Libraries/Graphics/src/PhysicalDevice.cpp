module Peio.Graphics.PhysicalDevice;

import Peio.Essentials.Types;

void Peio::Graphics::PhysicalDevice::Init()
{
    uint deviceCount;
    vk::Result result = instance.Get().enumeratePhysicalDevices(&deviceCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw FailedDeviceEnumerationException(result);
    }
    if (deviceCount == 0) {
        throw NoAvailableDevicesException();
    }

    std::vector<vk::PhysicalDevice> devices(deviceCount);
    result = instance.Get().enumeratePhysicalDevices(&deviceCount, &devices[0]);
    if (result != vk::Result::eSuccess) {
        throw FailedDeviceEnumerationException(result);
    }

    physicalDevice = devices[0];
}

vk::PhysicalDevice& Peio::Graphics::PhysicalDevice::Get()
{
    return physicalDevice;
}