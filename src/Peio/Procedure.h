#pragma once

#include "PeioHeader.h"
#include <functional>

namespace Peio {

	template <typename... T_args>
	struct Procedure : public std::function<void(T_args...)> {
		using _function = std::function<void(T_args...)>;
		using _function::function;

		template <typename... T_to>
		operator Procedure<T_to...>() {
			static_assert(sizeof...(T_to) == sizeof...(T_args), "Can't convert between argument lengths.");
			static_assert(((std::is_convertible<T_to, T_args>::value || IsDynamicallyConvertible<T_to, T_args>::value) && ...), "Can't convert arguments.");
			return [this](T_to... args) {
				if (!((!IsDynamicallyConvertible<T_to, T_args>::value || (!args || (args = reinterpret_cast<T_to>(dynamic_cast<T_args>(args))))), ...)) // Set dynamically casted arguments to their casts to avoid double casting (only if the argument is not nullptr).
					return;
				this->operator()(((T_args)args)...);
			};
		}
	};

}