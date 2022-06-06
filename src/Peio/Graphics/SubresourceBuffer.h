#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	template <typename T_element>
	struct SubresourceBuffer {

		_NODISCARD T_element* GetSubresourceBuffer() const noexcept {
			return buffer;
		}

		_NODISCARD T_element& operator[](size_t ind) const noexcept {
			return buffer[ind];
		}

		_NODISCARD size_t GetBufferSize() const noexcept {
			return resourceData.RowPitch;
		}

		_NODISCARD size_t GetNumElements() const noexcept {
			return resourceData.RowPitch / sizeof(T_element);
		}

		_NODISCARD D3D12_SUBRESOURCE_DATA GetResourceData() const noexcept {
			return resourceData;
		}

		_NODISCARD operator D3D12_SUBRESOURCE_DATA() const noexcept {
			return resourceData;
		}

		void SetBuffer(T_element* elements, size_t numElements) {
			buffer = elements;
			resourceData.pData = buffer;
			resourceData.RowPitch = numElements * sizeof(T_element);
			resourceData.SlicePitch = numElements * sizeof(T_element);
		}

		void SetBuffer(T_element* elements, size_t elementsPerRow, size_t numRows) {
			buffer = elements;
			resourceData.pData = buffer;
			resourceData.RowPitch = elementsPerRow * sizeof(T_element);
			resourceData.SlicePitch = elementsPerRow * sizeof(T_element) * numRows;
		}

		void Allocate(size_t numElements) {
			SetBuffer(new T_element[numElements], numElements);
		}

		void Release() {
			resourceData = {};
			if (buffer) {
				delete[] buffer;
				buffer = nullptr;
			}
		}

		~SubresourceBuffer() {
			Release();
		}

	protected:

		T_element* buffer = nullptr;
		D3D12_SUBRESOURCE_DATA resourceData = {};

	};

}