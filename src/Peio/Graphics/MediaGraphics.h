#pragma once

#include "Graphics.h"
#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT MediaGraphics : public Graphics {

		void Init(Long2 size, UINT numBuffers);

		void Clear(const Float4& color) override;
		void Render() override;

		~MediaGraphics();

		_NODISCARD byte* GetBuffer() const;

	protected:

		std::vector<Resource> readbackResources = {};
		std::vector<CD3DX12_TEXTURE_COPY_LOCATION> copySrc = {};
		std::vector<CD3DX12_TEXTURE_COPY_LOCATION> copyDst = {};
		std::vector<byte*> buffers = {};
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT bufferFootprint = {};
		D3D12_RANGE mapRange = {};

	};

}