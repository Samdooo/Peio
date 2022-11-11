#pragma once

#include "KeyboardListener.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT RawKeyEvent : public WinEvent {
		bool foreground;
		USHORT key;
		RawKeyEvent(MSG msg, bool foreground, USHORT key) : WinEvent(msg), foreground(foreground), key(key) {}
	};
	struct PEIO_WIN_EXPORT RawKeyDownEvent : public RawKeyEvent { using RawKeyEvent::RawKeyEvent; };
	struct PEIO_WIN_EXPORT RawKeyUpEvent : public RawKeyEvent { using RawKeyEvent::RawKeyEvent; };

	struct PEIO_WIN_EXPORT RawKeyboardListener : public Listener {

		static void Register(HWND hwnd);

	};

	extern PEIO_WIN_EXPORT Listener rawKeyboardListener;

}