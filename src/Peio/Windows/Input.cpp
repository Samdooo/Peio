#define PEIO_WIN_EXPORTING
#include "Input.h"

namespace Peio::Win {

	std::unordered_map<size_t, Procedure<WinEvent*>> Input::listeners = {};

	LRESULT Input::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WinMessageEvent message = {};
		message.msg = { hwnd, msg, wParam, lParam };
		message.returnDefaultProc = true;
		message.returnValue = 0;

		HandleEvent(&message);

		if (message.returnDefaultProc)
			return DefWindowProc(hwnd, msg, wParam, lParam);
		else
			return message.returnValue;
	}

	void Input::HandleEvent(WinEvent* event)
	{
		for (auto& listener : listeners)
			listener.second(event);
	}

}