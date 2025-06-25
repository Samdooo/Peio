module Peio.GraphicsQueue;

void Peio::Graphics::Queue::InitGraphics()
{
	device.Get().getQueue(queueFamilies.GetGraphicsIndex(), 0, &queue);
}