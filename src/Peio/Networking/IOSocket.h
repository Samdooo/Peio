#pragma once

#include "EventSocket.h"

namespace Peio::Net {

	template <typename T_sock>
	struct ReceiveSocket : public EventSocket<T_sock, ReceiveSocket, FD_READ_BIT> {
		int Receive(char* buf, int length) {
			return recv(Socket<>::sock, buf, length, 0);
		}
	};
	using ReceiveEvent = SocketEvent<ReceiveSocket>;

	template <typename T_sock>
	struct SendSocket : public EventSocket<T_sock, SendSocket, FD_WRITE_BIT> {
		int Send(const char* buf, int length) {
			return send(Socket<>::sock, buf, length, 0);
		}
	};
	using SendEvent = SocketEvent<SendSocket>;

}