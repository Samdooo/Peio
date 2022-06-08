#pragma once

#include "WindowsHeader.h"
#include "..\EventHandler.h"

#include <unordered_set>
#include <unordered_map>
#include <typeinfo>

namespace Peio::Win {

	struct PEIO_WIN_EXPORT Input {

		struct PEIO_WIN_EXPORT Message : public MSG {
			bool returnDefaultProc = true;
			LRESULT returnValue = 0;
		};

		static void AddListener(EventHandler<Message>* listener);

		template <typename... T_events>
		static void AddEventHandler(EventHandler<T_events...>* eventHandler) {
			(eventHandlers[typeid(T_events).hash_code()].insert(static_cast<EventHandler<>*>(eventHandler)), ...);
		}

		template <typename... T_events>
		static void RemoveEventHandler(EventHandler<T_events...>* eventHandler) {
			(eventHandlers.at(typeid(T_events).hash_code()).erase(static_cast<EventHandler<>*>(eventHandler)), ...);
		}

		template <typename T_event>
		static void Handle(T_event& event) {
			if (!eventHandlers.count(typeid(T_event).hash_code()))
				return;
			std::unordered_set<EventHandler<>*>& handlers = eventHandlers.at(typeid(T_event).hash_code());
			for (EventHandler<>* handler : handlers)
				handler->Handle(event);
		}

		template <typename T_event>
		static void HandleNew(T_event event) {
			Handle(event);
		}

		static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

	private:

		static std::vector<EventHandler<Message>*> listeners;
		static std::unordered_map<size_t, std::unordered_set<EventHandler<>*>> eventHandlers;

	};

	using Listener = PEIO_WIN_EXPORT EventHandler<Input::Message>;

}