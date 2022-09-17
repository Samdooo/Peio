#include "..\App.h"

namespace Graphics {

	void Window::Init()
	{
		window.CreateClass("Peio Voxel Editor Class", 0);
		window.RegisterClass();
		window.CreateWindow("Peio Voxel Editor", WS_POPUP | WS_VISIBLE, WS_EX_ACCEPTFILES, { 100, 100 }, windowSize);
	}

	void Window::Update()
	{
		window.HandleMessages();
	}

}