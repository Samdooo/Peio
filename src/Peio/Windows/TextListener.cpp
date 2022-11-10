#define PEIO_WIN_EXPORTING
#include "TextListener.h"

namespace Peio::Win {

	Procedure<WinEvent*> textListener = Listener([](WinMessageEvent* event)
	{
		if (event->msg.message == WM_CHAR) {
			Input::HandleNewEvent(TextEvent{ event->msg, (char)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
		}
		return false;
	});

}