#pragma once

#include "Global.h"
#include "..\Files.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT Shader {

		static D3D12_SHADER_BYTECODE Load(const char* path);

	};

}