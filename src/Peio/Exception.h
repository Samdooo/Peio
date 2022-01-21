#pragma once

#include "PeioHeader.h"

#define PEIO_EXCEPTION(x) Peio::Exception(x, __FILE__, __LINE__)

namespace Peio {

	struct Exception {

		Exception(const char* msg, const char* file, int line) : msg(msg), file(file), line(line) {}

		const char* msg;
		const char* file;
		const int line;

	};

}