module Peio.Graphics.Windows.PhysicalDevice;

bool Peio::Graphics::Windows::PhysicalDevice::IsSuitable() const
{
	return suitable;
}

void Peio::Graphics::Windows::PhysicalDevice::Init()
{
	std::vector<QueueFamily> families = GetQueueFamilies();

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