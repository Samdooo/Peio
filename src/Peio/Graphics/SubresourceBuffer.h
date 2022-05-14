#pragma once

#include "GraphicsHeader.h"

namespace Peio::Gfx {

	template <typename T_element>
	struct SubresourceBuffer {

		_NODISCARD T_element* GetSubresourceBuffer() const noexcept {
			return buffer;
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

		void SetBuffer(T_element* elements, size_t numElements) {
			buffer = elements;
			resourceData.pData = buffer;
			resourceData.RowPitch = numElements * sizeof(T_element);
			resourceData.SlicePitch = numElements * sizeof(T_element);
		}

		void Allocate(size_t numElements) {
			SetBuffer(new T_element[numElements], numElements);
		}

		void Release() {
			resourceData = {};
			PEIO_SAFE_DELETEALL(buffer);
		}

		~SubresourceBuffer() {
			Release();
		}

	protected:

		T_element* buffer = nullptr;
		D3D12_SUBRESOURCE_DATA resourceData = {};

	};

}