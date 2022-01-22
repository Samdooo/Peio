#pragma once

#include "WindowsHeader.h"
#include "..\EventHandler.h"

#include <unordered_map>

namespace Peio::Win {

	struct PEIO_WIN_EXPORT Input {

		struct PEIO_WIN_EXPORT Message : public MSG {
			bool returnDefaultProc = true;
			LRESULT returnValue = 0;
		};

		static void AddListener(EventHandler<Message>* listener);

		template <typename... T_events>
		static void AddEventHandler(EventHandler<T_events...>* eventHandler) {
			(eventHandlers.insert(std::pair(typeid(T_events).hash_code(), static_cast<EventHandler<>*>(eventHandler))), ...);
		}

		template <typename T_event>
		static void Handle(T_event& event) {
			auto its = eventHandlers.equal_range(typeid(T_event).hash_code());
			for (auto it = its.first; it != its.second; it++) {
				it->second->Handle(event);
			}
		}

		template <typename T_event>
		static void HandleNew(T_event event) {
			Handle(event);
		}

		static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

	private:

		static std::vector<EventHandler<Message>*> listeners;
		static std::unordered_multimap<size_t, EventHandler<>*> eventHandlers;

	};

}