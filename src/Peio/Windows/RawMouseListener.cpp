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

	void RawMouseListener::Handle(Input::Message& msg)
	{
		if (msg.message == WM_INPUT) {
			RAWINPUT* pInput = nullptr;
			UINT inputSize = 0;
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

			pInput = (RAWINPUT*)malloc(inputSize);

			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, pInput, &inputSize, sizeof(RAWINPUTHEADER));

			RAWINPUT input = *pInput;

			if (input.header.dwType != RIM_TYPEMOUSE) {
				return;
			}
			bool foreground = !msg.wParam;

			if (input.data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				Input::HandleNew(RawMouseMoveEvent{ foreground, { input.data.mouse.lLastX, input.data.mouse.lLastY } });

			if (input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
				Input::HandleNew(RawMouseButtonDownEvent{ foreground, MouseButton::LEFT });
			else if (input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
				Input::HandleNew(RawMouseButtonUpEvent{ foreground, MouseButton::LEFT });

			if (input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
				Input::HandleNew(RawMouseButtonDownEvent{ foreground, MouseButton::RIGHT });
			else if (input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
				Input::HandleNew(RawMouseButtonUpEvent{ foreground, MouseButton::RIGHT });

			if (input.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
				Input::HandleNew(RawMouseButtonDownEvent{ foreground, MouseButton::MIDDLE });
			else if (input.data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
				Input::HandleNew(RawMouseButtonUpEvent{ foreground, MouseButton::MIDDLE });
		}
	}

}