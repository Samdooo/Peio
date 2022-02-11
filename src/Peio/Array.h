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
		CommonArray(Ts... vals) {
			static_assert(sizeof...(Ts) == length, "Wrong number of arguments given to CommonArray.");
			size_t i = 0;
			((_Elems[i++] = (T)vals), ...);
		}

		template <typename TT>
		CommonArray(const CommonArray<TT, length>& that) {
			for (size_t i = 0; i < length; i++)
				_Elems[i] = (T)that[i];
		}

		template <typename TT>
		CommonArray<T, length>& operator=(const CommonArray<TT, length>& that) {
			for (int i = 0; i < length; i++)
				_Elems[i] = (T)that[i];
			return *this;
		}

		CommonArray(const std::initializer_list<T>& l) {
			memcpy(_Elems, l.begin(), sizeof(_Elems));
		}

		CommonArray<T, length>& operator=(const std::initializer_list<T>& l) {
			memcpy(_Elems, l.begin(), sizeof(_Elems));
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
			for (size_t i = 0; i < length - 1; i++)
				result += std::to_string(_Elems[i]) + ", ";
			result += std::to_string(_Elems[length - 1]) + "]";
			return result;
		}

	};

	template <typename T, size_t length>
	struct Array : public CommonArray<T, length> {
		using CommonArray<T, length>::CommonArray;
	};

	template <typename T>
	struct Array<T, 1> : public CommonArray<T, 1> {
		using CommonArray<T, 1>::CommonArray;
		using CommonArray<T, 1>::_Elems;
		_NODISCARD T& x() noexcept {
			return _Elems[0];
		}
		_NODISCARD const T& x() const noexcept {
			return _Elems[0];
		}
	};

	template <typename T>
	struct Array<T, 2> : public CommonArray<T, 2> {
		using CommonArray<T, 2>::CommonArray;
		using CommonArray<T, 2>::_Elems;
		_NODISCARD T& x() noexcept {
			return _Elems[0];
		}
		_NODISCARD const T& x() const noexcept {
			return _Elems[0];
		}
		_NODISCARD T& y() noexcept {
			return _Elems[1];
		}
		_NODISCARD const T& y() const noexcept {
			return _Elems[1];
		}
	};

	template <typename T>
	struct Array<T, 3> : public CommonArray<T, 3> {
		using CommonArray<T, 3>::CommonArray;
		using CommonArray<T, 3>::_Elems;
		_NODISCARD T& x() noexcept {
			return _Elems[0];
		}
		_NODISCARD const T& x() const noexcept {
			return _Elems[0];
		}
		_NODISCARD T& y() noexcept {
			return _Elems[1];
		}
		_NODISCARD const T& y() const noexcept {
			return _Elems[1];
		}
		_NODISCARD T& z() noexcept {
			return _Elems[2];
		}
		_NODISCARD const T& z() const noexcept {
			return _Elems[2];
		}
	};

	using Float2 = Array<float, 2>;
	using Float3 = Array<float, 3>;
	using Float4 = Array<float, 4>;

	using Int2 = Array<int, 2>;
	using Int3 = Array<int, 3>;
	using Int4 = Array<int, 4>;

	using Uint2 = Array<uint, 2>;
	using Uint3 = Array<uint, 3>;
	using Uint4 = Array<uint, 4>;

	using Long2 = Array<long, 2>;
	using Long3 = Array<long, 3>;
	using Long4 = Array<long, 4>;

	using Byte2 = Array<byte, 2>;
	using Byte3 = Array<byte, 3>;
	using Byte4 = Array<byte, 4>;

}