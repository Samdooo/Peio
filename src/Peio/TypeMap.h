#pragma once

#include "PeioHeader.h"

namespace Peio {

	template <typename... Ts>
	struct TypeIndex {

	protected:

		template <size_t T_index, typename... T_rest>
		struct Type {
			using type = void;
		};

		template <size_t T_index, typename T_first, typename... T_rest>
		struct Type<T_index, T_first, T_rest...> : public Type<T_index - 1, T_rest...> {};

		template <typename T_first, typename... T_rest>
		struct Type<0, T_first, T_rest...> {
			using type = T_first;
		};

		template <typename T_target, size_t T_index, typename... T_rest>
		struct Index {
			static constexpr size_t index = -1;
		};

		template <typename T_target, size_t T_index, typename T_first, typename... T_rest>
		struct Index<T_target, T_index, T_first, T_rest...> : public Index<T_target, T_index + 1, T_rest...> {};

		template <typename T_target, size_t T_index, typename... T_rest>
		struct Index<T_target, T_index, T_target, T_rest...> {
			static constexpr size_t index = T_index;
		};

	public:

		/// <summary>
		/// Static index-to-type conversion. Returns void if T_index is out of bounds.
		/// </summary>
		template <size_t T_index>
		using type = Type<T_index, Ts...>::type;

		/// <summary>
		/// Static type-to-index conversion. Returns (size_t)(-1) if T_target doesn't exist in Ts.
		/// </summary>
		template <typename T_target>
		static constexpr size_t index = Index<T_target, 0, Ts...>::index;

	};

	/// <summary>
	/// A static class for static and dynamic conversions between indices and types. <para/>
	/// Example: <para/> TypeMap&lt;Base, Child1, Child2&gt;::New[1]() <para/>
	/// Generates a new Child2 object and returns it as a Base pointer, assuming Child2 inherits Base.
	/// </summary>
	template <typename T_base, typename... Ts>
	struct TypeMap : public TypeIndex<Ts...> {

		/// <summary>
		/// Dynamic index-to-type conversion.
		/// </summary>
		static constexpr T_base* (*New[])() = {
		([]() { return static_cast<T_base*>(new Ts); })...
		};

	};

}