#pragma once

#include "MediaHeader.h"

#define PEIO_MED_EXCEPTION(x, y) Peio::Med::Exception(x, y, __FILE__, __LINE__)

namespace Peio::Med {

	struct Exception {

		Exception(const char* msg, int ret, const char* file, int line) : msg(msg), ret(ret), file(file), line(line) {}

		const char* msg;
		const char* file;
		const int line;
		const int ret;

	};

}