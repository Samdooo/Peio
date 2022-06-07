#pragma once

#include "Array.h"

namespace Peio {

	template <typename T, size_t length>
	struct Offset : public Array<T, length> {

		using Array<T, length>::Array;

		Offset<T, length>* parent = nullptr;

		_NODISCARD Array<T, length> GetTranslated() const noexcept {
			if (parent)
				return parent->GetTranslated() + *this;
			else
				return *this;
		}

	};

}