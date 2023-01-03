#pragma once

#include "Global.h"

namespace Peio::Gfx {

		class PEIO_GFX_EXPORT SwapChain {

		protected:

			ComPtr<IDXGIFactory2> factory = nullptr;
			ComPtr<IDXGISwapChain3> swapChain = nullptr;

		public:

			void Init(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen);
			void Resize(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen);
			void SetFullscreenState(BOOL fullscreen);

			void GetBuffer(UINT buffer, ID3D12Resource** renderTarget);
			UINT SwapBuffers();

			~SwapChain();

		};

}