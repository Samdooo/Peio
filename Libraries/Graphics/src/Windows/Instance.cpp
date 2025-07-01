module Peio.Graphics.Windows.Instance;

void Peio::Graphics::Instances::Windows::Init(const std::string& appName)
{
	instance.Init(appName, InstanceExtensions::windowsGraphics);

	std::vector<Graphics::PhysicalDevice> physicalDevices = Instances::PhysicalDeviceEnum(instance).GetPhysicalDevices();
	
	bool foundSuitable = false;
	for (Graphics::PhysicalDevice& physicalDevice : physicalDevices) {
		this->physicalDevice.Init(physicalDevice.Get());

		if (this->physicalDevice.IsSuitable()) {
			foundSuitable = true;
			break;
		}
	}
	if (!foundSuitable) {
		throw NoSuitablePhysicalDeviceException();
	}
}