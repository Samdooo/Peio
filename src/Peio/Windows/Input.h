#pragma once

#include "WindowsHeader.h"
#include "..\Procedure.h"
#include "..\Exception.h"

#include <unordered_set>
#include <typeinfo>

namespace Peio::Win {

	struct PEIO_WIN_EXPORT WinEvent {
		MSG msg;
		WinEvent(MSG msg = {}) : msg(msg) {}
		virtual ~WinEvent() {}
	};

	struct PEIO_WIN_EXPORT WinMessageEvent : public WinEvent {
		bool returnDefaultProc;
		LRESULT returnValue;
		WinMessageEvent(MSG msg = {}, bool returnDefaultProc = true, LRESULT returnValue = 0) : WinEvent(msg), returnDefaultProc(returnDefaultProc), returnValue(returnValue) {}
	};

	struct PEIO_WIN_EXPORT Input {
		
		static ProcedureSet<WinEvent*> listeners;

		static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

		template <typename T_event>
		static void HandleNewEvent(T_event event) {
			listeners(&event);
		}

	};

	using Listener = PEIO_WIN_EXPORT Procedure<WinMessageEvent*>;

}