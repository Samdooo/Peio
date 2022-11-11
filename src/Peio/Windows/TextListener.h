#pragma once

#include "Input.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT TextEvent : public WinEvent {
		char character;
		ushort repeat;
		bool prev;
		TextEvent(MSG msg, char character, ushort repeat, bool prev) : WinEvent(msg), character(character), repeat(repeat), prev(prev) {}
	};

	//struct PEIO_WIN_EXPORT TextListener : public Listener {
	//
	//	bool Handle(WinMessageEvent* event) override;
	//
	//};

	extern PEIO_WIN_EXPORT Listener textListener;

}