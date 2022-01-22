#pragma once

#include "MouseListener.h"

namespace Peio::Win {

	struct RawMouseMoveEvent {
		bool foreground;
		Long2 movement;
	};
	struct RawMouseButtonDownEvent {
		bool foreground;
		MouseButton button;
	};
	struct RawMouseButtonUpEvent : public RawMouseButtonDownEvent {};

	struct PEIO_WIN_EXPORT RawMouseListener : public virtual Listener {

		static void Register(HWND hwnd);

		void Handle(Input::Message& msg) override;

	};

}