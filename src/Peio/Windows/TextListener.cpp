#define PEIO_WIN_EXPORTING
#include "TextListener.h"

namespace Peio::Win {

	void TextListener::Handle(Input::Message& msg)
	{
		if (msg.message == WM_CHAR) {
			Input::HandleNew(TextEvent{ (char)msg.wParam, LOWORD(msg.lParam), bool(msg.lParam & (1 << 30)) });
		}
	}

}