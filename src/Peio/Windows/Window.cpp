#define PEIO_WIN_EXPORTING
#include "Window.h"

namespace Peio::Win {

	void Window::CreateClass(std::string name, UINT styles, WNDPROC proc)
	{
		std::wstring wName(CA2W(name.c_str()));

		wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = styles;
		wndClass.lpfnWndProc = proc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = nullptr;
		wndClass.hIcon = nullptr;
		wndClass.hCursor = nullptr;
		wndClass.hbrBackground = nullptr;
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = wName.c_str();
		wndClass.hIconSm = nullptr;
	}

	void Window::RegisterClass()
	{
		if (RegisterClassEx(&wndClass) == 0) {
			throw PEIO_WIN_EXCEPTION("Failed to register window class.");
		}
	}

	void Window::CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size, HWND parent, HMENU menu, LPVOID lpParam)
	{
		std::wstring wTitle(CA2W(title.c_str()));

		hwnd = CreateWindowEx(exStyles, wndClass.lpszClassName, wTitle.c_str(), styles
			, position[0], position[1], size[0], size[1], parent, menu, nullptr, lpParam);
		if (!hwnd) {
			throw PEIO_WIN_EXCEPTION("Failed to create window.");
		}
	}

	void Window::SetAbsoluteStyles(LONG styles)
	{
		if (SetWindowLong(hwnd, GWL_STYLE, styles) == 0) {
			throw PEIO_WIN_EXCEPTION("Failed to set absolute window styles.");
		}
	}

	void Window::SetAbsoluteExStyles(LONG styles)
	{
		if (SetWindowLong(hwnd, GWL_EXSTYLE, styles) == 0) {
			throw PEIO_WIN_EXCEPTION("Failed to set absolute window exstyles.");
		}
	}

	void Window::HandleMessages()
	{
		MSG msg = {};
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	RECT Window::GetRect()
	{
		RECT result;
		GetWindowRect(hwnd, &result);
		return result;
	}

	void Window::Remap(Int2 position, Int2 size)
	{
		HDWP hdwp = BeginDeferWindowPos(1);
		DeferWindowPos(hdwp, hwnd, NULL, position.x(), position.y(), size.x(), size.y(), SWP_NOZORDER);
		EndDeferWindowPos(hdwp);
	}

	void Window::SetOpacity(BYTE opacity)
	{
		SetLayeredWindowAttributes(hwnd, NULL, opacity, LWA_ALPHA);
	}

	HWND Window::GetHWND() const noexcept
	{
		return hwnd;
	}

	WNDCLASSEX Window::GetWindowClass() const noexcept
	{
		return wndClass;
	}

	Window::~Window()
	{
		if (hwnd) {
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
	}

	void Window::Show()
	{
		ShowWindow(hwnd, SW_SHOW);
	}

	void Window::ShowMinimized()
	{
		ShowWindow(hwnd, SW_SHOWMINIMIZED);
	}

	void Window::ShowMaximized()
	{
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	}

	void Window::Hide()
	{
		ShowWindow(hwnd, SW_HIDE);
	}

	void Window::Minimize()
	{
		ShowWindow(hwnd, SW_MINIMIZE);
	}

	void Window::Restore()
	{
		ShowWindow(hwnd, SW_RESTORE);
	}

	void Window::Close()
	{
		CloseWindow(hwnd);
	}

}