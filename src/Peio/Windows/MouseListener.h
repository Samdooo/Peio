#pragma once

#include "Input.h"
#include "..\Array.h"

namespace Peio::Win {

	enum class MouseButton { LEFT, RIGHT, MIDDLE, X1, X2 };
	enum class MouseKey {
		CONTROL = MK_CONTROL,
		LEFT_MOUSE = MK_LBUTTON,
		MIDDLE_MOUSE = MK_MBUTTON,
		RIGHT_MOUSE = MK_RBUTTON,
		SHIFT = MK_SHIFT,
		X1_MOUSE = MK_XBUTTON1,
		X2_MOUSE = MK_XBUTTON2
	};

	struct PEIO_WIN_EXPORT MouseMoveEvent {
		Int2 position;
		WPARAM wParam;
	};

	struct PEIO_WIN_EXPORT MouseButtonDownEvent {
		MouseButton button;
		Int2 position;
		WPARAM wParam;
	};

	struct PEIO_WIN_EXPORT MouseButtonUpEvent : public MouseButtonDownEvent {};

	struct PEIO_WIN_EXPORT MouseWheelEvent {
		short delta;
		Int2 position;
		WPARAM wParam;
	};

	struct PEIO_WIN_EXPORT MouseListener : public virtual Listener {

		void Handle(Input::Message& msg);

	};

}