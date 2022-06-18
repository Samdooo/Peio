#define PEIO_WIN_EXPORTING
#include "Input.h"

namespace Peio::Win {

	std::vector<EventHandler<WinMessageEvent>*> Input::listeners = {};
	BaseHandlerSet<WinEvent> Input::eventHandlers = {};
	//HandlerSet Input::eventHandlers.Handlers;

	void Input::AddListener(EventHandler<WinMessageEvent>* listener)
	{
		listeners.push_back(listener);
	}

	LRESULT Input::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WinMessageEvent message = {};
		message.msg = { hwnd, msg, wParam, lParam };
		message.returnDefaultProc = true;
		message.returnValue = 0;

		for (EventHandler<WinMessageEvent>* listener : listeners) {
			listener->Handle(&message);
		}

		if (message.returnDefaultProc)
			return DefWindowProc(hwnd, msg, wParam, lParam);
		else
			return message.returnValue;
	}

}