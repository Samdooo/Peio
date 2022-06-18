#define PEIO_WIN_EXPORTING
#include "KeyboardListener.h"

namespace Peio::Win {

	bool KeyboardListener::Handle(WinMessageEvent* event)
	{
		switch (event->msg.message) {
		case WM_KEYDOWN:
			Input::eventHandlers.HandleNew(KeydownEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		case WM_KEYUP:
			Input::eventHandlers.HandleNew(KeyupEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		default:
			break;
		}
		return false;
	}

}