#pragma once

#include "PeioHeader.h"
#include <functional>

template <typename T_ret, typename... T_args>
struct std::equal_to<std::function<T_ret(T_args...)>> {

	using PtrType = T_ret(*)(T_args...);

	bool operator()(std::function<T_ret(T_args...)> left, std::function<T_ret(T_args...)> right) const {
		PtrType* lPtr = left.template target<PtrType>();
		PtrType* rPtr = right.template target<PtrType>();
		return *lPtr == *rPtr;
	}

};

template <typename T_ret, typename... T_args>
struct std::hash<std::function<T_ret(T_args...)>> {

	using PtrType = T_ret(*)(T_args...);

	size_t operator()(std::function<T_ret(T_args...)> fnc) const {
		PtrType* ptr = fnc.template target<PtrType>();
		return std::hash<size_t>()((size_t)*ptr);
	}

};