module;

#include <Windows.h>
#include <hidusage.h>
#include <windowsx.h>

module Peio.Windows.Mouse;

Peio::Windows::MessageHandler Peio::Windows::CreateMouseMoveHandler(Proc<MouseMoveEvent&> onMouseMove)
{
	return [onMouseMove](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_MOUSEMOVE) {
			MouseMoveEvent event = {};
			event.msg = msgEvent.msg;
			event.pos = { GET_X_LPARAM(msgEvent.msg.lParam), GET_Y_LPARAM(msgEvent.msg.lParam) };
			event.additional = (MouseAdditional)msgEvent.msg.wParam;
			onMouseMove(event);
		}
	};
}

Peio::Windows::MessageHandler Peio::Windows::CreateMouseDownHandler(Proc<MouseDownEvent&> onMouseDown)
{
	return [onMouseDown](MessageEvent& msgEvent) {
		MouseDownEvent event = {};
		switch (msgEvent.msg.message) {
		case WM_LBUTTONDOWN:
			event.button = MouseButton::Left;
			break;
		case WM_RBUTTONDOWN:
			event.button = MouseButton::Right;
			break;
		case WM_MBUTTONDOWN:
			event.button = MouseButton::Middle;
			break;
		case WM_XBUTTONDOWN:
			event.button = (HIWORD(msgEvent.msg.wParam) == XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
			break;
		default:
			return;
		}
		event.msg = msgEvent.msg;
		event.pos = { GET_X_LPARAM(msgEvent.msg.lParam), GET_Y_LPARAM(msgEvent.msg.lParam) };
		event.additional = (MouseAdditional)LOWORD(msgEvent.msg.wParam);
		onMouseDown(event);
	};
}

Peio::Windows::MessageHandler Peio::Windows::CreateMouseUpHandler(Proc<MouseUpEvent&> onMouseUp)
{
	return [onMouseUp](MessageEvent& msgEvent) {
		MouseUpEvent event = {};
		switch (msgEvent.msg.message) {
		case WM_LBUTTONUP:
			event.button = MouseButton::Left;
			break;
		case WM_RBUTTONUP:
			event.button = MouseButton::Right;
			break;
		case WM_MBUTTONUP:
			event.button = MouseButton::Middle;
			break;
		case WM_XBUTTONUP:
			event.button = (HIWORD(msgEvent.msg.wParam) == XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
			break;
		default:
			return;
		}
		event.msg = msgEvent.msg;
		event.pos = { GET_X_LPARAM(msgEvent.msg.lParam), GET_Y_LPARAM(msgEvent.msg.lParam) };
		event.additional = (MouseAdditional)LOWORD(msgEvent.msg.wParam);
		onMouseUp(event);
	};
}

Peio::Windows::MessageHandler Peio::Windows::CreateMouseWheelHandler(Proc<MouseWheelEvent&> onMouseWheel)
{
	return [onMouseWheel](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_MOUSEWHEEL) {
			MouseWheelEvent event = {};
			event.msg = msgEvent.msg;
			event.pos = { GET_X_LPARAM(msgEvent.msg.lParam), GET_Y_LPARAM(msgEvent.msg.lParam) };
			event.additional = (MouseAdditional)LOWORD(msgEvent.msg.wParam);
			event.delta = (short)HIWORD(msgEvent.msg.wParam);
			onMouseWheel(event);
		}
	};
}

void Peio::Windows::RegisterRawMouseInput(const Window& window)
{
	RAWINPUTDEVICE rid;
	rid.dwFlags = RIDEV_INPUTSINK; // Enable non-foreground input
	rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	rid.hwndTarget = window.GetHWND();

	BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	if (!ret) {
		throw FailedRawMouseInputRegistrationException();
	}
}

Peio::Windows::MessageHandler Peio::Windows::CreateRawMouseHandler(Proc<RawMouseMoveEvent&> onRawMouseMove, 
	Proc<RawMouseDownEvent&> onRawMouseDown, Proc<RawMouseUpEvent&> onRawMouseUp,
	Proc<RawMouseWheelEvent&> onRawMouseWheel)
{
	return [onRawMouseMove, onRawMouseDown, onRawMouseUp, onRawMouseWheel](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_INPUT) {
			UINT inputSize = 0;
			GetRawInputData((HRAWINPUT)msgEvent.msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER)); // Query size

			RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
			GetRawInputData((HRAWINPUT)msgEvent.msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

			if (input->header.dwType != RIM_TYPEMOUSE) {
				free(input);
				return;
			}
			bool foreground = !msgEvent.msg.wParam;

			if (input->data.mouse.usFlags == MOUSE_MOVE_RELATIVE && onRawMouseMove) {
				RawMouseMoveEvent event = {};
				event.msg = msgEvent.msg;
				event.foreground = foreground;
				event.movement = { input->data.mouse.lLastX, input->data.mouse.lLastY };
				onRawMouseMove(event);
			}

			RawMouseDownEvent downEvent = {};
			RawMouseUpEvent upEvent = {};

			downEvent.msg = msgEvent.msg;
			downEvent.foreground = foreground;
			upEvent.msg = msgEvent.msg;
			upEvent.foreground = foreground;

			if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN && onRawMouseDown) {
				downEvent.button = MouseButton::Left;
				onRawMouseDown(downEvent);
			}
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP && onRawMouseUp) {
				upEvent.button = MouseButton::Left;
				onRawMouseUp(upEvent);
			}
				
			if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN && onRawMouseDown) {
				downEvent.button = MouseButton::Right;
				onRawMouseDown(downEvent);
			}
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP && onRawMouseUp) {
				upEvent.button = MouseButton::Right;
				onRawMouseUp(upEvent);
			}

			if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN && onRawMouseDown) {
				downEvent.button = MouseButton::Middle;
				onRawMouseDown(downEvent);
			}
			else if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP && onRawMouseUp) {
				upEvent.button = MouseButton::Middle;
				onRawMouseUp(upEvent);
			}

			if (input->data.mouse.usButtonFlags & RI_MOUSE_WHEEL && onRawMouseWheel) {
				RawMouseWheelEvent event = {};
				event.msg = msgEvent.msg;
				event.foreground = foreground;

				event.delta = (short)input->data.mouse.usButtonData;

				onRawMouseWheel(event);
			}

			free(input);

		}
	};
}
