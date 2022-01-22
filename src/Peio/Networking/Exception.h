#pragma once

#include "NetworkingHeader.h"
#include "..\Exception.h"

#define PEIO_NET_EXCEPTION(x) Peio::NET::Exception(x, __FILE__, __LINE__)

namespace Peio::NET {

	struct Exception {

		Exception(const char* msg, const char* file, int line) : msg(msg), file(file), line(line), wsaError(WSAGetLastError()) {}

		const char* msg;
		const char* file;
		const int line;
		const int wsaError;

	};
	
}