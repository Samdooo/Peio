#define PEIO_WIN_EXPORTING
#include "RawKeyboardListener.h"

namespace Peio::Win {

	void RawKeyboardListener::Register(HWND hwnd)
	{
		RAWINPUTDEVICE rid;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.usUsagePage = 1;
		rid.usUsage = 6;
		rid.hwndTarget = hwnd;

		BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	}

	Listener rawKeyboardListener = [](WinMessageEvent* event)
	{
		if (event->msg.message == WM_INPUT) {
			UINT inputSize = 0;
			GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
			GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

			if (input->header.dwType != RIM_TYPEKEYBOARD) {
				free(input);
				return false;
			}
			bool foreground = !event->msg.wParam;

			if ((input->data.keyboard.Flags & 1) == 0) {
				Input::HandleNewEvent(RawKeyDownEvent{ event->msg, foreground, input->data.keyboard.VKey });
			}
			else if ((input->data.keyboard.Flags & 1) == 1) {
				Input::HandleNewEvent(RawKeyUpEvent{ event->msg, foreground, input->data.keyboard.VKey });
			}
			free(input);
		}
		return false;
	};

}