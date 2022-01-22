#pragma once

#include "EventSocket.h"

namespace Peio::Net {

	template <typename T_sock>
	struct ListenSocket : public EventSocket<T_sock, ListenSocket, FD_ACCEPT_BIT> {
		void Listen(int backlog = SOMAXCONN) {
			if (listen(Socket<>::sock, backlog) != 0) {
				throw PEIO_NET_EXCEPTION("Failed to listen on socket.");
			}
		}
	};
	using AcceptEvent = SocketEvent<ListenSocket>;

}