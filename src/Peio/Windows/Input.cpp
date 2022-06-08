#define PEIO_WIN_EXPORTING
#include "Input.h"

namespace Peio::Win {

	std::vector<EventHandler<Input::Message>*> Input::listeners;
	std::unordered_map<size_t, std::unordered_set<EventHandler<>*>> Input::eventHandlers;

	void Input::AddListener(EventHandler<Message>* listener)
	{
		listeners.push_back(listener);
	}

	LRESULT Input::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Message message = { hwnd, msg, wParam, lParam, true, 0 };

		for (EventHandler<Message>* listener : listeners) {
			listener->Handle(message);
		}

		if (message.returnDefaultProc)
			return DefWindowProc(hwnd, msg, wParam, lParam);
		else
			return message.returnValue;
	}

}