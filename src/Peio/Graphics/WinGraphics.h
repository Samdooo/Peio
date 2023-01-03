#pragma once

#include "Graphics.h"
#include "SwapChain.h"

namespace Peio::Gfx {

	class PEIO_GFX_EXPORT WinGraphics : public Graphics {

	protected:

		SwapChain swapChain = {};

	public:

		void Init(HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen);
		void Resize(HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen);

		void Clear(const Float4& color) override;
		void Render() override;

		~WinGraphics();

	};

}