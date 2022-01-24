#pragma once

#include "VoxelsHeader.h"

namespace Peio::Vxl {

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

		void Allocate(size_t n_elements) {
			Release();
			buffer = new T_element[n_elements];
			resourceData.pData = buffer;
			resourceData.RowPitch = n_elements * sizeof(T_element);
			resourceData.SlicePitch = n_elements * sizeof(T_element);
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