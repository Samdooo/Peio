#pragma once

#include "Input.h"
#include "..\Array.h"

namespace Peio::Win {

	enum class PEIO_WIN_EXPORT MouseButton { LEFT, RIGHT, MIDDLE, X1, X2 };
	enum class PEIO_WIN_EXPORT MouseKey {
		CONTROL = MK_CONTROL,
		LEFT_MOUSE = MK_LBUTTON,
		MIDDLE_MOUSE = MK_MBUTTON,
		RIGHT_MOUSE = MK_RBUTTON,
		SHIFT = MK_SHIFT,
		X1_MOUSE = MK_XBUTTON1,
		X2_MOUSE = MK_XBUTTON2
	};

	struct PEIO_WIN_EXPORT MouseMoveEvent : public WinEvent {
		Int2 position;
		MouseMoveEvent(MSG msg, Int2 position) : WinEvent(msg), position(position) {}
	};

	struct PEIO_WIN_EXPORT MouseButtonEvent : public WinEvent {
		MouseButton button;
		Int2 position;
		MouseButtonEvent(MSG msg, MouseButton button, Int2 position) : WinEvent(msg), button(button), position(position) {}
	};

	struct PEIO_WIN_EXPORT MouseButtonDownEvent : public MouseButtonEvent { using MouseButtonEvent::MouseButtonEvent; };
	struct PEIO_WIN_EXPORT MouseButtonUpEvent : public MouseButtonEvent { using MouseButtonEvent::MouseButtonEvent; };

	struct PEIO_WIN_EXPORT MouseWheelEvent : public WinEvent {
		short delta;
		Int2 position;
		MouseWheelEvent(MSG msg, short delta, Int2 position) : WinEvent(msg), delta(delta), position(position) {}
	};

	//struct PEIO_WIN_EXPORT MouseListener : public Listener {
	//
	//	bool Handle(WinMessageEvent* event) override;
	//
	//};

	extern PEIO_WIN_EXPORT Procedure<WinEvent*> mouseListener;

}