#define PEIO_WIN_EXPORTING
#include "KeyboardListener.h"

namespace Peio::Win {

	void KeyboardListener::Handle(Input::Message& msg)
	{
		switch (msg.message) {
		case WM_KEYDOWN:
			Input::HandleNew(KeydownEvent{ (uchar)msg.wParam, LOWORD(msg.lParam), bool(msg.lParam & (1 << 30)) });
			break;
		case WM_KEYUP:
			Input::HandleNew(KeyupEvent{ (uchar)msg.wParam, LOWORD(msg.lParam), bool(msg.lParam & (1 << 30)) });
			break;
		default:
			break;
		}
	}

}