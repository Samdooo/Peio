#pragma once

#include "WindowsHeader.h"
#include "..\EventHandler.h"

#include <unordered_set>
#include <unordered_map>
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

		static void AddListener(EventHandler<WinMessageEvent>* listener);

		//template <typename... T_events>
		//static void AddEventHandler(EventHandler<T_events...>* eventHandler) {
		//	(eventHandlers[typeid(T_events).hash_code()].insert(static_cast<EventHandler<>*>(eventHandler)), ...);
		//}
		//
		//template <typename... T_events>
		//static void RemoveEventHandler(EventHandler<T_events...>* eventHandler) {
		//	(eventHandlers.at(typeid(T_events).hash_code()).erase(static_cast<EventHandler<>*>(eventHandler)), ...);
		//}
		//
		//template <typename T_event>
		//static void Handle(T_event& event) {
		//	if (!eventHandlers.count(typeid(T_event).hash_code()))
		//		return;
		//	std::unordered_set<EventHandler<>*>& handlers = eventHandlers.at(typeid(T_event).hash_code());
		//	for (EventHandler<>* handler : handlers)
		//		handler->Handle(event);
		//}
		//
		//template <typename T_event>
		//static void HandleNew(T_event event) {
		//	Handle(event);
		//}

		static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

		//static HandlerSet handlers;
		static BaseHandlerSet<WinEvent> eventHandlers;

	protected:

		static std::vector<EventHandler<WinMessageEvent>*> listeners;

	};

	using Listener = PEIO_WIN_EXPORT EventHandler<WinMessageEvent>;

}