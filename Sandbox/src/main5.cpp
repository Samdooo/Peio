#include <Peio/Windows/Input.h>
#include <Peio/Windows/RawKeyboardListener.h>
#include <Peio/Windows/Window.h>
#include <iostream>
#include <Peio/Windows/KeyboardListener.h>

#define KEY_RESET VK_RETURN
#define KEY_SETFOCUS VK_ADD
#define MOUSEPOS_RESETWORLD 1800, 1040
#define MOUSEPOS_BACKTOGAME 960, 330

int main() {

	std::vector<HWND> windows = {};

	EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		WCHAR wstr[1024];
		GetWindowText(hwnd, wstr, 1024);
		if (std::wstring(wstr) == L"Minecraft* 1.16.5 - Singleplayer") {
			((std::vector<HWND>*)lParam)->push_back(hwnd);
		}
		return TRUE;
		}, (LPARAM)&windows);
	if (windows.size() == 0) {
		std::cout << "No instances found." << std::endl;
		return 0;
	}
	std::cout << "Found " << windows.size() << " instances" << std::endl;

	Peio::Win::Window window;
	window.CreateClass("Peio Sandbox", 0);
	window.RegisterClass();
	window.CreateWindow("Peio Sandbox", 0, 0, {}, {});

	Peio::Win::RawKeyboardListener::Register(window.GetHWND());

	Peio::Win::RawKeyboardListener keyboardListener;
	Peio::Win::Input::AddListener(&keyboardListener);

	UINT currentWindow = 0;
	Peio::FunctionHandler<Peio::Win::RawKeyDownEvent> keyboardHandler(
		[&windows, &currentWindow, &window](Peio::Win::RawKeyDownEvent* event) -> bool {
			//if (event->key == KEY_RESET) {
			//	Sleep(50);
			//	SetCursorPos(MOUSEPOS_RESETWORLD);
			//	Sleep(50);
			//	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			//	Sleep(50);
			//	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			//	Sleep(50);
			//	ShowWindow(windows[currentWindow], SW_SHOWMINIMIZED);
			//	currentWindow++;
			//	currentWindow %= windows.size();
			//}
			//if (event->key == KEY_SETFOCUS || event->key == KEY_RESET) {
			//	ShowWindow(windows[currentWindow], SW_SHOWMAXIMIZED);
			//	SetCursorPos(MOUSEPOS_BACKTOGAME);
			//	Sleep(50);
			//}
			if (event->key == KEY_RESET || event->key == KEY_SETFOCUS) {
				if (event->key == KEY_RESET) {
					ShowWindow(windows[currentWindow], SW_SHOWMINIMIZED);
					currentWindow++;
					currentWindow %= windows.size();
				}
				ShowWindow(windows[currentWindow], SW_SHOWMAXIMIZED);
				SetCursorPos(MOUSEPOS_BACKTOGAME);
			}
			return false;
		}
	);

	Peio::Win::Input::eventHandlers.Insert(&keyboardHandler, keyboardHandler.GetBaseHandler<Peio::Win::WinEvent>());

	while (true) {
		window.HandleMessages();
	}

	return 0;
}