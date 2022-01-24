#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	template <typename T>
	class SubresourceVector : public std::vector<T> {

	protected:

		D3D12_SUBRESOURCE_DATA subresource = {};

	public:

		using std::vector<T>::vector;

		void UpdateSubresource() {
			subresource.pData = &std::vector<T>::at(0);
			subresource.RowPitch = sizeof(T) * std::vector<T>::size();
			subresource.SlicePitch = subresource.RowPitch;
		}

		const D3D12_SUBRESOURCE_DATA& GetSubresource() const noexcept {
			return subresource;
		}

	};

}