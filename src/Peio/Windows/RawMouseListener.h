#pragma once

#include "MouseListener.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT RawMouseMoveEvent : public WinEvent {
		bool foreground;
		Long2 movement;
		RawMouseMoveEvent(MSG msg, bool foreground, Long2 movement) : WinEvent(msg), foreground(foreground), movement(movement) {}
	};
	struct PEIO_WIN_EXPORT RawMouseButtonEvent : public WinEvent {
		bool foreground;
		MouseButton button;
		RawMouseButtonEvent(MSG msg, bool foreground, MouseButton button) : WinEvent(msg), foreground(foreground), button(button) {}
	};
	struct PEIO_WIN_EXPORT RawMouseWheelEvent : public WinEvent {
		bool foreground;
		SHORT delta;
		RawMouseWheelEvent(MSG msg, bool foreground, SHORT delta) : WinEvent(msg), foreground(foreground), delta(delta) {}
	};

	struct PEIO_WIN_EXPORT RawMouseButtonDownEvent : public RawMouseButtonEvent { using RawMouseButtonEvent::RawMouseButtonEvent;  };
	struct PEIO_WIN_EXPORT RawMouseButtonUpEvent : public RawMouseButtonEvent { using RawMouseButtonEvent::RawMouseButtonEvent; };

	struct PEIO_WIN_EXPORT RawMouseListener : public Listener {

		static void Register(HWND hwnd);

	};

	extern PEIO_WIN_EXPORT Listener rawMouseListener;

}