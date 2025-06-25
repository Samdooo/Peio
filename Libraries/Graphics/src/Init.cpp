module Peio.Graphics.Init;

void Peio::Graphics::Init()
{
    instance.Init("Peio Graphics");
    physicalDevice.Init();
    queueFamilies.Init();
    device.Init();
}

void Peio::Graphics::Cleanup()
{
    device.Destroy();
    instance.Destroy();
}
