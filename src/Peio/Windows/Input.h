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

		static void AddListener(Procedure<WinEvent*>* listener);
		static void RemoveListener(Procedure<WinEvent*>* listener);

		static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
		static void HandleEvent(WinEvent* event);

		template <typename T_event>
		static void HandleNewEvent(T_event event) {
			HandleEvent(&event);
		}

	protected:

		static std::unordered_set<Procedure<WinEvent*>*> listeners;

	};

	using Listener = PEIO_WIN_EXPORT Procedure<WinMessageEvent*>;

}