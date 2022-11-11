#define PEIO_WIN_EXPORTING
#include "RawMouseListener.h"

namespace Peio::Win {

	void RawMouseListener::Register(HWND hwnd)
	{
		RAWINPUTDEVICE rid;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.usUsagePage = 1;
		rid.usUsage = 2;
		rid.hwndTarget = hwnd;

		BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	}

	Listener rawMouseListener = [](WinMessageEvent* event)
	{
		if (event->msg.message == WM_INPUT) {
			UINT inputSize = 0;
			GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
			GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

			if (input->header.dwType != RIM_TYPEMOUSE) {
				return false;
			}
			bool foreground = !event->msg.wParam;

			if (input->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				Input::HandleNewEvent(RawMouseMoveEvent{ event->msg, foreground, { input->data.mouse.lLastX, input->data.mouse.lLastY } });

			if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
				Input::HandleNewEvent(RawMouseButtonDownEvent{ event->msg, foreground, MouseButton::LEFT });
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
				Input::HandleNewEvent(RawMouseButtonUpEvent{ event->msg, foreground, MouseButton::LEFT });

			if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
				Input::HandleNewEvent(RawMouseButtonDownEvent{ event->msg, foreground, MouseButton::RIGHT });
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
				Input::HandleNewEvent(RawMouseButtonUpEvent{ event->msg, foreground, MouseButton::RIGHT });

			if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
				Input::HandleNewEvent(RawMouseButtonDownEvent{ event->msg, foreground, MouseButton::MIDDLE });
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
				Input::HandleNewEvent(RawMouseButtonUpEvent{ event->msg, foreground, MouseButton::MIDDLE });

			delete input;
		}
		return false;
	};

}