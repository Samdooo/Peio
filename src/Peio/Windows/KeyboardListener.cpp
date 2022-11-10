#define PEIO_WIN_EXPORTING
#include "KeyboardListener.h"

namespace Peio::Win {

	Procedure<WinEvent*> keyboardListener = Listener([](WinMessageEvent* event) {
		switch (event->msg.message) {
		case WM_KEYDOWN:
			Input::HandleNewEvent(KeydownEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		case WM_KEYUP:
			Input::HandleNewEvent(KeyupEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		default:
			break;
		}
	});

}