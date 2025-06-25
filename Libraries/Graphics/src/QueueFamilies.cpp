module Peio.Graphics.QueueFamilies;

void Peio::Graphics::QueueFamilies::Init()
{
	uint familyCount;
	physicalDevice.Get().getQueueFamilyProperties(&familyCount, nullptr);

	families.resize(familyCount);
	physicalDevice.Get().getQueueFamilyProperties(&familyCount, &families[0]);

	graphicsInd = -1;
	for (uint i = 0; i < familyCount; i++) {
		if (families[i].queueFlags & vk::QueueFlagBits::eGraphics)
			graphicsInd = i;
	}

	if (graphicsInd == -1) {
		throw FamilyNotFoundException();
	}
}

Peio::uint Peio::Graphics::QueueFamilies::GetGraphicsIndex() const
{
	return graphicsInd;
}
