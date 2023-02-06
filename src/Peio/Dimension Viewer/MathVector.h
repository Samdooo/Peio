#pragma once

#include <vector>
#include "..\Exception.h"

template <typename T>
struct MathVector : public std::vector<T> {

	using _vector = std::vector<T>;
	using _vector::vector;

#define MATHVECTOR_OPERATOR(x)												\
	MathVector<T>& operator x##=(const std::vector<T>& that) {				\
		if (that.size() != _vector::size()) {								\
			throw PEIO_EXCEPTION("Sizes of vectors are not equal.");		\
		}																	\
		for (size_t i = 0; i < _vector::size(); i++) {						\
			_vector::at(i) x##= that[i];									\
		}																	\
		return *this;														\
	}																		\
	MathVector<T>& operator x##=(T val) {									\
		for (size_t i = 0; i < _vector::size(); i++) {						\
			_vector::at(i) x##= val;										\
		}																	\
		return *this;														\
	}																		\
	MathVector<T> operator##x##(const std::vector<T>& that) {				\
		MathVector<T> result(*this);										\
		result x##= that;													\
		return result;														\
	}																		\
	MathVector<T> operator##x##(T val) {									\
		MathVector<T> result(*this);										\
		result x##= val;													\
		return result;														\
	}
	
	MATHVECTOR_OPERATOR(+);
	MATHVECTOR_OPERATOR(-);
	MATHVECTOR_OPERATOR(*);
	MATHVECTOR_OPERATOR(/);
	MATHVECTOR_OPERATOR(%);
	MATHVECTOR_OPERATOR(<<);
	MATHVECTOR_OPERATOR(>>);
	MATHVECTOR_OPERATOR(|);
	MATHVECTOR_OPERATOR(&);
	MATHVECTOR_OPERATOR(^);

	MathVector<T>& Rotate(const std::vector<T>& v) {
		if (v.size() != _vector::size() - 1) {
			throw PEIO_EXCEPTION("Incorrect size of input vector.");
		}
		for (size_t i = 0; i < _vector::size() - 1; i++) {
			T c = (T)cos(v[i]);
			T s = (T)sin(v[i]);
			T x = _vector::at(0) * c - _vector::at(i + 1) * s;
			T y = _vector::at(i + 1) * c + _vector::at(0) * s;
			_vector::at(0) = x;
			_vector::at(i + 1) = y;
		}
		return *this;
	}

};