#pragma once

#include "EventSocket.h"

namespace Peio::Net {

	template <typename T_sock>
	struct CloseSocket : public EventSocket<T_sock, CloseSocket, FD_CLOSE_BIT> {};
	using CloseEvent = SocketEvent<CloseSocket>;

}