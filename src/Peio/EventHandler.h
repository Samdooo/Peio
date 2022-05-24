#pragma once

#include "Exception.h"
#include <functional>

namespace Peio {

	template <typename... T_events>
	struct EventHandler : public EventHandler<T_events>... {};

	template <>
	struct EventHandler<> {

		template <typename T_event>
		void Handle(T_event& e) {
			EventHandler<T_event>* handler = dynamic_cast<EventHandler<T_event>*>(this);
			if (!handler) {
				return;
				//throw PEIO_EXCEPTION("Invalid event type or non-virtual EventHandler inheritance in an EventPipeline.");
			}
			handler->Handle(e);
		}

		virtual ~EventHandler() {}

	};

	template <typename T_event>
	struct EventHandler<T_event> : public virtual EventHandler<> {

		virtual void Handle(T_event&) = 0;

	protected:

		using EventHandler<>::Handle;

	};

	template <typename... T_events>
	struct EventHandler<EventHandler<T_events...>> : public EventHandler<T_events>... {};

	template <typename T_event, typename... T_handlers>
	struct EventPipeline : public T_handlers... {
		static_assert((std::is_base_of<EventHandler<T_event>, T_handlers>::value && ...), "All T_handlers must virtually inherit EventHandler<T_event>.");

		void Handle(T_event& event) override {
			(T_handlers::Handle(event), ...);
		}
	};

	template <typename... T_events>
	struct FunctionHandler : public FunctionHandler<T_events>... {
		
		FunctionHandler(std::function<void(T_events&)>... functions) : FunctionHandler<T_events>(functions)... {}

		using FunctionHandler<T_events>::Handle...;

	};

	template <typename T_event>
	struct FunctionHandler<T_event> : public EventHandler<T_event> {

		FunctionHandler(std::function<void(T_event&)> function) : function(function) {}

		void Handle(T_event& event) override {
			function(event);
		}

	protected:

		std::function<void(T_event&)> function = nullptr;

	};

}