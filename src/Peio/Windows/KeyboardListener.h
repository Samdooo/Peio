#pragma once

#include "Input.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT KeyboardEvent : public WinEvent {
		uchar key;
		ushort repeat;
		bool prev;
		KeyboardEvent(MSG msg, uchar key, ushort repeat, bool prev) : WinEvent(msg), key(key), repeat(repeat), prev(prev) {}
	};

	struct PEIO_WIN_EXPORT KeydownEvent : public KeyboardEvent { using KeyboardEvent::KeyboardEvent; };
	struct PEIO_WIN_EXPORT KeyupEvent : public KeyboardEvent { using KeyboardEvent::KeyboardEvent; };

	//struct PEIO_WIN_EXPORT KeyboardListener : public Listener {
	//
	//	bool Handle(WinMessageEvent* event) override;
	//
	//};

	extern PEIO_WIN_EXPORT Procedure<WinEvent*> keyboardListener;

}