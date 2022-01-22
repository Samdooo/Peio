#pragma once

#include "GraphicsHeader.h"

#define PEIO_GFX_EXCEPTION(x, y) Peio::Net::Exception(x, __FILE__, __LINE__, y)

namespace Peio::Gfx {

	struct Exception {

		Exception(const char* msg, const char* file, int line, HRESULT ret)
			: msg(msg), file(file), line(line), ret(ret), d3d12Error(GetLastError()) {}

		const char* msg;
		const char* file;
		const int line;
		const HRESULT ret;
		const int d3d12Error;

	};

}