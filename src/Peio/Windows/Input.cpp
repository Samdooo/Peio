#define PEIO_WIN_EXPORTING
#include "Input.h"

namespace Peio::Win {

	ProcedureSet<WinEvent*> Input::listeners = {};

	LRESULT Input::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WinMessageEvent message = {};
		message.msg = { hwnd, msg, wParam, lParam };
		message.returnDefaultProc = true;
		message.returnValue = 0;

		listeners(&message);

		if (message.returnDefaultProc)
			return DefWindowProc(hwnd, msg, wParam, lParam);
		else
			return message.returnValue;
	}

}