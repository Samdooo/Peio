module;

#include <Windows.h>
#include <hidusage.h>
#include <windowsx.h>

module Peio.Windows.Keyboard;

Peio::Windows::MessageHandler Peio::Windows::CreateKeyDownHandler(Proc<KeyDownEvent&> onKeyDown)
{
	return [onKeyDown](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_KEYDOWN) {
			KeyDownEvent event{};
			event.msg = msgEvent.msg;
			event.key = (uchar)msgEvent.msg.wParam;
			event.repeatCount = LOWORD(msgEvent.msg.lParam);
			event.prevState = (bool)(msgEvent.msg.lParam & (1 << 30));
			onKeyDown(event);
		}
	};
}

Peio::Windows::MessageHandler Peio::Windows::CreateKeyUpHandler(Proc<KeyUpEvent&> onKeyUp)
{
	return [onKeyUp](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_KEYUP) {
			KeyUpEvent event{};
			event.msg = msgEvent.msg;
			event.key = (uchar)msgEvent.msg.wParam;
			event.repeatCount = LOWORD(msgEvent.msg.lParam);
			event.prevState = (bool)(msgEvent.msg.lParam & (1 << 30));
			onKeyUp(event);
		}
	};
}

Peio::Windows::MessageHandler Peio::Windows::CreateCharHandler(Proc<CharEvent&> onChar)
{
	return [onChar](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_CHAR) {
			CharEvent event{};
			event.msg = msgEvent.msg;
			event.key = (uchar)msgEvent.msg.wParam;
			event.repeatCount = LOWORD(msgEvent.msg.lParam);
			event.prevState = (bool)(msgEvent.msg.lParam & (1 << 30));
			onChar(event);
		}
	};
}

void Peio::Windows::RegisterRawKeyboardInput(const Window& window)
{
	RAWINPUTDEVICE rid;
	rid.dwFlags = RIDEV_INPUTSINK; // Enable non-foreground input
	rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rid.hwndTarget = window.GetHWND();

	BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	if (!ret) {
		throw FailedRawKeyboardInputRegistrationException();
	}
}

Peio::Windows::MessageHandler Peio::Windows::CreateRawKeyHandler(Proc<RawKeyDownEvent&> onRawKeyDown, Proc<RawKeyUpEvent&> onRawKeyUp)
{
	return [onRawKeyDown, onRawKeyUp](MessageEvent& msgEvent) {
		if (msgEvent.msg.message == WM_INPUT) {
			UINT inputSize = 0;
			GetRawInputData((HRAWINPUT)msgEvent.msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER)); // Query size

			RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
			GetRawInputData((HRAWINPUT)msgEvent.msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

			if (input->header.dwType != RIM_TYPEKEYBOARD) {
				free(input);
				return;
			}
			bool foreground = !msgEvent.msg.wParam;

			RawKeyEvent event{};
			event.msg = msgEvent.msg;
			event.foreground = foreground;
			event.key = input->data.keyboard.VKey;
			event.invalid = (input->data.keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE || input->data.keyboard.VKey >= UCHAR_MAX);
			event.scanCode = MAKEWORD(input->data.keyboard.MakeCode & 0x7f,
				((input->data.keyboard.Flags & RI_KEY_E0) ? 0xe0 : ((input->data.keyboard.Flags & RI_KEY_E1) ? 0xe1 : 0x00)));
			event.winMessage = input->data.keyboard.Message;

			if (input->data.keyboard.Flags & RI_KEY_BREAK) {
				if (onRawKeyUp) {
					RawKeyUpEvent upEvent(event);
					onRawKeyUp(upEvent);
				}
			}
			else {
				if (onRawKeyDown) {
					RawKeyDownEvent downEvent(event);
					onRawKeyDown(downEvent);
				}
			}

			free(input);

		}
	};
}