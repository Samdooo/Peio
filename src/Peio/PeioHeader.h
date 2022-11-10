#pragma once

#include <type_traits>
#include <stdint.h>

#ifdef PEIO_EXPORTING
#define PEIO_EXPORT __declspec(dllexport)
#else
#define PEIO_EXPORT __declspec(dllimport)
#endif

#pragma warning(disable : 4250) /* Disables warnings for when functions are being inherited via dominance. */
#pragma warning(disable : 4251) /* Disables warnings for when using STL stuff in dll's, requires the same debugger level throughout projects. */

#define PEIO_SAFE_DELETE(x) if (x) delete (x)
#define PEIO_SAFE_DELETEALL(x) if (x) delete[] (x)

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

	template <typename T_from, typename T_to>
	struct IsDynamicallyConvertible {
		static constexpr bool value = !std::is_convertible<T_from, T_to>::value && std::is_convertible<T_to, T_from>::value &&
			std::is_pointer<T_from>::value && std::is_pointer<T_to>::value &&
			std::has_virtual_destructor<std::remove_pointer<T_from>::type>::value;
	};

}