module Peio.Graphics.Windows.PhysicalDevice;

bool Peio::Graphics::PhysicalDevices::Windows::IsSuitable() const
{
	return suitable;
}

void Peio::Graphics::PhysicalDevices::Windows::Init(vk::PhysicalDevice physicalDevice)
{
	this->physicalDevice.Init(physicalDevice);

	std::vector<QueueFamily> families = this->physicalDevice.GetQueueFamilies();

	bool found = false;
	for (QueueFamily& family : families) {
		if (family.GetProperties().queueFlags & vk::QueueFlagBits::eGraphics) {
			found = true;
			familyIndices.graphics = family.GetIndex();
			break;
		}
	}
	if (!found)
		suitable = false;
}