module Peio.Graphics.Windows.Instance;

void Peio::Graphics::Windows::Instance::Init(const std::string& appName)
{
	this->Graphics::Instance::Init(appName, InstanceExtensions::windowsGraphics);

	std::vector<Graphics::PhysicalDevice> physicalDevices = Instances::PhysicalDeviceEnum(*this).GetPhysicalDevices();
	
	bool foundSuitable = false;
	for (Graphics::PhysicalDevice& physicalDevice : physicalDevices) {
		(Graphics::PhysicalDevice&)this->physicalDevice = physicalDevice;
		this->physicalDevice.Init();

		if (this->physicalDevice.IsSuitable()) {
			foundSuitable = true;
			break;
		}
	}
	if (!foundSuitable) {
		throw NoSuitablePhysicalDeviceException();
	}
}