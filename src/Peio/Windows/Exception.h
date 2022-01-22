#pragma once

#include "WindowsHeader.h"

#define PEIO_WIN_EXCEPTION(x) Peio::Win::Exception(x, __FILE__, __LINE__)

namespace Peio::Win {

	struct Exception {

		Exception(const char* msg, const char* file, int line) : msg(msg), file(file), line(line), winError(GetLastError()) {}

		const char* msg;
		const char* file;
		const int line;
		const int winError;

	};

}