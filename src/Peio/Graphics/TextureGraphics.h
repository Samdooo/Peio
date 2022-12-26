#pragma once

#include "Graphics.h"
#include "RenderTargetHeap.h"
#include "RenderTexture.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT TextureGraphics : public Graphics {

		void Init(Long2 size);

		void Clear(const Float4& color) override;
		void Render() override;

		_NODISCARD const RenderTexture& GetTexture() const noexcept;
		_NODISCARD const DescriptorHeap& GetSrvHeap() const noexcept;
		_NODISCARD const DescriptorHeap& GetRtvHeap() const noexcept;

	protected:

		RenderTexture texture = {};
		DescriptorHeap srvHeap = {};
		DescriptorHeap rtvHeap = {};

	};

}