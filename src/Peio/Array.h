#pragma once

#include "PeioHeader.h"

#include <array>
#include <initializer_list>
#include <string>

namespace Peio {

	template <typename T, size_t length>
	struct CommonArray : public std::array<T, length> {
		static_assert(length, "Length must be greater than 0.");

		using _array = std::array<T, length>;
		using _array::_Elems;
		using _array::array;

		template <typename... Ts>
		CommonArray(T first, Ts... vals) {
			/* 'first' is necessary to allow construction from an implicit template conversion.  */
			static_assert((std::is_convertible<Ts, T>::value && ...), "Wrong argument type(s) given to CommonArray.");
			static_assert(sizeof...(Ts) == (length - 1), "Wrong number of arguments given to CommonArray.");
			_Elems[0] = first;
			size_t i = 1;
			((_Elems[i++] = (T)vals), ...);
		}

		CommonArray(const CommonArray<T, length>& that) {
			operator=(that);
		}

		template <typename TT>
		CommonArray(const CommonArray<TT, length>& that) {
			operator=(that);
		}

		template <size_t ind = 0>
		CommonArray<T, length>& operator=(const CommonArray<T, length>& that) {
			_Elems[ind] = that[ind];
			if constexpr (ind < length - 1)
				operator=<ind + 1>(that);
			return *this;
		}

		template <typename TT, size_t ind = 0>
		CommonArray<T, length>& operator=(const CommonArray<TT, length>& that) {
			_Elems[ind] = (T)that[ind];
			if constexpr (ind < length - 1)
				operator=<TT, ind + 1>(that);
			return *this;
		}

		_NODISCARD constexpr size_t GetSize() const noexcept {
			return sizeof(_Elems);
		}

		_NODISCARD constexpr size_t GetStride() const noexcept {
			return sizeof(T);
		}

		_NODISCARD constexpr size_t GetLength() const noexcept {
			return length;
		}

		_NODISCARD constexpr byte* Bytes() const noexcept {
			return reinterpret_cast<byte*>(this);
		}

		_NODISCARD std::string ToString() const {
			std::string result = "[";
			for (size_t i = 0; i < length; i++) {
				if constexpr (std::is_convertible<T, std::string>::value)
					result += (std::string)_Elems[i];
				else
					result += std::to_string(_Elems[i]);

				if (i == length - 1)
					result += "]";
				else
					result += ", ";
			}
			return result;
		}

		/* Forgive me */
#define PEIO_COMMONARRAY_OPERATOR(x)													\
		template <size_t ind = 0>														\
		CommonArray<T, length>& operator x##=(const CommonArray<T, length>& that) {		\
			_Elems[ind] x##= that[ind];													\
			if constexpr (ind < length - 1)												\
				operator##x##=<ind + 1>(that);											\
			return *this;																\
		}																				\
		template <size_t ind = 0>														\
		CommonArray<T, length>& operator##x##=(T that) {								\
			_Elems[ind] x##= that;														\
			if constexpr (ind < length - 1)												\
				operator##x##=<ind + 1>(that);											\
			return *this;																\
		}																				\
		CommonArray<T, length> operator x (const CommonArray<T, length>& that) const {	\
			CommonArray<T, length> result(*this);										\
			result x##= that;															\
			return result;																\
		}																				\
		CommonArray<T, length> operator x (T that) const {								\
			CommonArray<T, length> result(*this);										\
			result x##= that;															\
			return result;																\
		}

		PEIO_COMMONARRAY_OPERATOR(+);
		PEIO_COMMONARRAY_OPERATOR(-);
		PEIO_COMMONARRAY_OPERATOR(*);
		PEIO_COMMONARRAY_OPERATOR(/);
		PEIO_COMMONARRAY_OPERATOR(%);
		PEIO_COMMONARRAY_OPERATOR(<<);
		PEIO_COMMONARRAY_OPERATOR(>>);
		PEIO_COMMONARRAY_OPERATOR(|);
		PEIO_COMMONARRAY_OPERATOR(&);
		PEIO_COMMONARRAY_OPERATOR(^);

	};

	template <typename T, size_t length>
	struct Array : public CommonArray<T, length> {
		using CommonArray<T, length>::CommonArray;
		Array(const CommonArray<T, length>& that) : CommonArray<T, length>(that) {}

#define PEIO_ARRAY_ELEM_ALIAS(x,i)					\
		_NODISCARD T& x##() noexcept {				\
			return _Elems[i];						\
		}											\
		_NODISCARD const T& x##() const noexcept {	\
			return _Elems[i];						\
		}
	};

	template <typename T>
	struct Array<T, 1> : public CommonArray<T, 1> {
		using CommonArray<T, 1>::CommonArray;
		using CommonArray<T, 1>::_Elems;
		Array(const CommonArray<T, 1>& that) : CommonArray<T, 1>(that) {}

		PEIO_ARRAY_ELEM_ALIAS(x, 0);
	};

	template <typename T>
	struct Array<T, 2> : public CommonArray<T, 2> {
		using CommonArray<T, 2>::CommonArray;
		using CommonArray<T, 2>::_Elems;
		Array(const CommonArray<T, 2>& that) : CommonArray<T, 2>(that) {}

		PEIO_ARRAY_ELEM_ALIAS(x, 0);
		PEIO_ARRAY_ELEM_ALIAS(y, 1);
	};

	template <typename T>
	struct Array<T, 3> : public CommonArray<T, 3> {
		using CommonArray<T, 3>::CommonArray;
		using CommonArray<T, 3>::_Elems;
		Array(const CommonArray<T, 3>& that) : CommonArray<T, 3>(that) {}

		PEIO_ARRAY_ELEM_ALIAS(x, 0);
		PEIO_ARRAY_ELEM_ALIAS(y, 1);
		PEIO_ARRAY_ELEM_ALIAS(z, 2);

		PEIO_ARRAY_ELEM_ALIAS(r, 0);
		PEIO_ARRAY_ELEM_ALIAS(g, 1);
		PEIO_ARRAY_ELEM_ALIAS(b, 2);
	};

	template <typename T>
	struct Array<T, 4> : public CommonArray<T, 4> {
		using CommonArray<T, 4>::CommonArray;
		using CommonArray<T, 4>::_Elems;
		Array(const CommonArray<T, 4>& that) : CommonArray<T, 4>(that) {}

		PEIO_ARRAY_ELEM_ALIAS(x, 0);
		PEIO_ARRAY_ELEM_ALIAS(y, 1);
		PEIO_ARRAY_ELEM_ALIAS(z, 2);
		PEIO_ARRAY_ELEM_ALIAS(w, 3);

		PEIO_ARRAY_ELEM_ALIAS(r, 0);
		PEIO_ARRAY_ELEM_ALIAS(g, 1);
		PEIO_ARRAY_ELEM_ALIAS(b, 2);
		PEIO_ARRAY_ELEM_ALIAS(a, 3);
	};

#define PEIO_ARRAY_ALIAS(x, n)  \
	using n##2 = Array< x , 2>;	\
	using n##3 = Array< x , 3>;	\
	using n##4 = Array< x , 4>;	

	PEIO_ARRAY_ALIAS(float, Float);
	PEIO_ARRAY_ALIAS(double, Double);
	PEIO_ARRAY_ALIAS(int, Int);
	PEIO_ARRAY_ALIAS(uint, Uint);
	PEIO_ARRAY_ALIAS(long, Long);
	PEIO_ARRAY_ALIAS(char, Char);
	PEIO_ARRAY_ALIAS(byte, Byte);

}