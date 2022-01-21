#pragma once

#include <type_traits>
#include <stdint.h>

#pragma warning(disable : 4250) /* Disables warnings for when functions are being inherited via dominance. */
#pragma warning(disable : 4251) /* Disables warnings for when using STL stuff in dll's, requires the same debugger level throughout projects. */

namespace Peio {

	using uchar = unsigned char;
	using byte = uchar;
	using uint = unsigned int;
	using ushort = unsigned short;

	template <template <typename...> typename Base, typename Class>
	struct is_template_of : public std::false_type {};

	template <template <typename...> typename Base, typename... Args>
	struct is_template_of<Base, Base<Args...>> : public std::true_type {};

	template < template <typename...> class Base, typename Derived>
	struct is_base_of_template_impl
	{
		template<typename... Ts>
		static constexpr std::true_type  Test(const Base<Ts...>*);
		static constexpr std::false_type Test(...);
		using type = decltype(Test(std::declval<Derived*>()));
	};

	template < template <typename...> class Base, typename Derived>
	using is_base_of_template = typename is_base_of_template_impl<Base, Derived>::type;

}