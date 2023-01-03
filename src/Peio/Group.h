#pragma once

#include "PeioHeader.h"

namespace Peio {

	template <typename... T_ptrs>
	struct Group : public Group<T_ptrs>... {

		Group() {}
		Group(T_ptrs*... ptrs) : Group<T_ptrs>(ptrs)... {}

		template <typename T_ptr>
		Group(T_ptr* ptr) : Group<T_ptrs>((T_ptrs*)ptr)... {}

		template <typename T_ptr>
		_NODISCARD T_ptr*& Get() noexcept {
			return Group<T_ptr>::Get();
		}

		template <typename T_ptr>
		_NODISCARD operator T_ptr*& () noexcept {
			return Get<T_ptr>();
		}

	};

	template <typename T_ptr>
	struct Group<T_ptr> {

		Group() {}
		Group(T_ptr* ptr) : ptr(ptr) {}

		_NODISCARD T_ptr*& Get() noexcept {
			return ptr;
		}

		_NODISCARD operator T_ptr*& () noexcept {
			return Get();
		}

	protected:

		T_ptr* ptr = nullptr;

	};

}