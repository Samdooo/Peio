#pragma once

#include "PeioHeader.h"
#include <functional>
#include <unordered_map>

namespace Peio {

	template <typename... T_args>
	struct Procedure : public std::function<void(T_args...)> {
		using _function = std::function<void(T_args...)>;
		using _function::function;

		template <typename... T_from>
		Procedure(Procedure<T_from...> from) : 
		_function(
			[from](T_args... args) {
				if (!(
					[&args]() {
						if constexpr (std::is_convertible<T_args, T_from>::value)
							return true;
						if (args == nullptr)
							return true;
						args = reinterpret_cast<T_args>(dynamic_cast<T_from>(args));
						return (args != nullptr);
					}()
					&& ...)) {
					return; // Arguments couldn't be dynamically converted.
				}
				from(((T_from)args)...);
			}
		) {
			static_assert(sizeof...(T_args) == sizeof...(T_from), "Can't convert between argument lengths.");
			static_assert(((std::is_convertible<T_args, T_from>::value || IsDynamicallyConvertible<T_args, T_from>::value) && ...), "Can't convert arguments.");
		}

	};

	template <typename... T_args>
	struct ProcedureSet : public Procedure<T_args...> {

		void Add(Procedure<T_args...> proc) {
			map.insert({ 0, proc });
		}

		template <typename... T_from>
		void Insert(Procedure<T_from...>* proc) {
			map.insert({ (size_t)proc, (Procedure<T_args...>)(*proc) });
		}

		template <typename... T_from>
		void Remove(Procedure<T_from...>* proc) {
			map.erase((size_t)proc);
		}

		template <typename... T_from>
		bool Contains(Procedure<T_from...>* proc) {
			return map.count((size_t)proc);
		}

		ProcedureSet() : Procedure<T_args...>(
			[this](T_args... args) {
				for (auto& proc : map)
					proc.second(args...);
			}) {}

	protected:

		std::unordered_multimap<size_t, Procedure<T_args...>> map = {};

	};

}