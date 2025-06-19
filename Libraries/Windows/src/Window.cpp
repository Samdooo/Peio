module;

#include <atlbase.h>
#include <atlconv.h>
#undef CreateWindow

module Peio.Windows.Window;

void Peio::Windows::CreateClass(std::string className, UINT classStyles, WNDPROC proc)
{
	std::wstring wClassName = std::wstring(CA2W(className.c_str()));

	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = classStyles;
	wndClass.lpfnWndProc = proc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = nullptr;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = wClassName.c_str();
	wndClass.hIconSm = nullptr;

	if (RegisterClassEx(&wndClass) == 0) {
		throw FailedClassRegistrationException();
	}
}

void Peio::Windows::Window::Create(std::string className, std::string title, Int2 size, DWORD styles, Int2 position, DWORD exStyles)
{
	std::wstring wClassName(CA2W(className.c_str()));
	std::wstring wTitle(CA2W(title.c_str()));

	hwnd = CreateWindowEx(exStyles, wClassName.c_str(), wTitle.c_str(), styles, position[0]
		, position[1], size[0], size[1], nullptr, nullptr, nullptr, nullptr);

	if (!hwnd) {
		throw FailedWindowCreationException();
	}
}

void Peio::Windows::Window::HandleMessages() const
{
	MSG msg = {};
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Peio::Windows::Window::Close() const
{
	if (CloseWindow(hwnd) == 0) {
		throw FailedWindowClosureException();
	}
}

void Peio::Windows::Window::Show() const
{
	ShowWindow(hwnd, SW_SHOW);
}

void Peio::Windows::Window::Hide() const
{
	ShowWindow(hwnd, SW_HIDE);
}

bool Peio::Windows::Window::IsOpen() const
{
	return IsWindow(hwnd);
}

HWND Peio::Windows::Window::GetHWND() const
{
	return hwnd;
}
