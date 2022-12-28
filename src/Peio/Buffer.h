#pragma once

#include "PeioHeader.h"
#include <vector>

namespace Peio {

	template <typename T_element>
	struct Buffer {

		Buffer() {
			data = {};
			allocated = false;
		}

		~Buffer() {
			if (allocated)
				vec.~vector();
		}

		_NODISCARD std::vector<T_element>& GetVector() noexcept {
			if (!allocated) {
				allocated = true;
				vec = std::vector<T_element>();
			}
			return vec;
		}

		void SetData(T_element* data, size_t size) noexcept {
			if (allocated) {
				allocated = false;
				vec.~vector();
			}
			this->data = { data, size };
		}

		_NODISCARD T_element* GetData() noexcept {
			if (allocated)
				return &vec[0];
			else
				return data.data;
		}
		_NODISCARD size_t GetSize() const noexcept {
			if (allocated)
				return vec.size();
			else
				return data.size;
		}

		_NODISCARD T_element& operator[](size_t ind) noexcept {
			if (allocated)
				return vec[ind];
			else
				return data.data[ind];
		}

	protected:

		bool allocated = false;
		union {
			std::vector<T_element> vec;
			struct { T_element* data = nullptr; size_t size = 0; } data = {};
		};

	};

}