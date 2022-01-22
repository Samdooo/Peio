#pragma once

#include "EventSocket.h"
#include "Hint.h"

namespace Peio::Net {

	template <typename T_sock>
	struct AcceptSocket : public T_sock {

		void Accept(SOCKET listener) {
			int length = sizeof(global);
			Socket<>::sock = accept(listener, global.GetSockaddr(), &length);
			if (Socket<>::sock == INVALID_SOCKET && WSAGetLastError() != WSAEWOULDBLOCK) {
				throw PEIO_NET_EXCEPTION("Failed to accept connection.");
			}
		}

	protected:

		Hint global = {};

	};

}