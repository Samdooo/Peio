#define PEIO_NET_EXPORTING
#include "Init.h"

namespace Peio::Net {

	void Init()
	{
		WSADATA data = {};
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
			throw PEIO_NET_EXCEPTION("Failed to initialize Winsock.");
		}
	}

}