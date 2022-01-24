#pragma once

#include "GraphicsHeader.h"
#include "Exception.h"

namespace Peio::Gfx {

		extern PEIO_GFX_EXPORT Microsoft::WRL::ComPtr<ID3D12Device> device;

		void PEIO_GFX_EXPORT Init();

}