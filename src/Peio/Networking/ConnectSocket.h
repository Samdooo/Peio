#pragma once

#include "EventSocket.h"
#include "Hint.h"

namespace Peio::Net {

	template <typename T_sock>
	struct ConnectSocket : public EventSocket<T_sock, ConnectSocket, FD_CONNECT_BIT> {

		void Connect(const Hint& global) {
			if (connect(Socket<>::sock, global.GetSockaddr(), sizeof(global)) != 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
				throw PEIO_NET_EXCEPTION("Failed to connect socket.");
			}
			this->global = global;
		}

	protected:

		Hint global = {};

	};
	using ConnectEvent = SocketEvent<ConnectSocket>;

}