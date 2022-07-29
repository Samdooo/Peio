#pragma once

#include "Exception.h"
#include <functional>
#include <unordered_map>

namespace Peio {

	template <typename T_base>
	using BaseHandler = std::function<bool(T_base*)>;

	template <typename... T_events>
	struct EventHandler : public virtual EventHandler<T_events>... {

		template <typename T_base>
		constexpr bool HandleBase(T_base* base) {
			return (this->EventHandler<T_events>::HandleBase<T_base>(base) || ...);
		}

		template <typename T_base>
		_NODISCARD constexpr BaseHandler<T_base> GetBaseHandler() {
			return [this](T_base* base) -> bool {
				return this->HandleBase<T_base>(base);
			};
		}

	};

	template <>
	struct PEIO_EXPORT EventHandler<> {

		template <typename T_event>
		bool Handle(T_event* event) {
			EventHandler<T_event>* handler = dynamic_cast<EventHandler<T_event>*>(this);
			if (handler) {
				return handler->Handle(event);
			}
			return false;
		}

		virtual ~EventHandler() {}

	};

	template <typename T_event>
	struct EventHandler<T_event> : public virtual EventHandler<> {

		virtual bool Handle(T_event*) = 0;

		template <typename T_base>
		constexpr bool HandleBase(T_base* base) {
			if constexpr (std::is_convertible<T_base*, T_event*>::value) {
				return Handle(base);
			}
			if constexpr (std::has_virtual_destructor<T_base>::value) { // Assuming base != nullptr
				T_event* event = dynamic_cast<T_event*>(base);
				if (event) {
					return Handle(event);
				}
			}
			return false;
		}

		template <typename T_base>
		_NODISCARD constexpr BaseHandler<T_base> GetBaseHandler() {
			return [this](T_base* base) -> bool {
				return this->HandleBase<T_base>(base);
			};
		}

	protected:

		using EventHandler<>::Handle;

	};

	template <typename... T_events>
	struct FunctionHandler : public virtual EventHandler<T_events...>, public virtual FunctionHandler<T_events>... {

		FunctionHandler(BaseHandler<T_events>... functions) : FunctionHandler<T_events>(functions)... {}

		using FunctionHandler<T_events>::Handle...;

	};

	template <typename T_event>
	struct FunctionHandler<T_event> : public virtual EventHandler<T_event> {

		BaseHandler<T_event> function;

		FunctionHandler(BaseHandler<T_event> function) : function(function) {}

		bool Handle(T_event* event) override {
			return function(event);
		}

	};

	template <typename T_base>
	struct BaseHandlerSet : public virtual EventHandler<T_base> {

		bool Handle(T_base* base) override {
			for (auto& handler : map) {
				handler.second(base);
			}
			return false;
		}

		template <typename T_event>
		bool HandleNew(T_event event) {
			return Handle(&event);
		}

		void Insert(EventHandler<>* handler, BaseHandler<T_base> baseHandler) {
			map.insert({ handler, baseHandler });
		}

		void Remove(EventHandler<>* handler, BaseHandler<T_base> baseHandler) {
			if (!map.count(handler)) {
				throw PEIO_EXCEPTION("EventHandler doesn't belong to BaseHandlerSet.");
			}
			map.erase(handler);
		}

	protected:

		std::unordered_map<EventHandler<>*, BaseHandler<T_base>> map = {};

	};

}