#define PEIO_WIN_EXPORTING
#include "Input.h"

namespace Peio::Win {

	std::unordered_set<Procedure<WinEvent*>*> Input::listeners = {};

	void Input::AddListener(Procedure<WinEvent*>* listener)
	{
		listeners.insert(listener);
	}

	void Input::RemoveListener(Procedure<WinEvent*>* listener)
	{
		if (!listeners.count(listener)) {
			throw PEIO_EXCEPTION("EventHandler not found in current set.");
		}
		listeners.erase(listener);
	}

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
		for (Procedure<WinEvent*>* listener : listeners)
			listener->operator()(event);
	}

}