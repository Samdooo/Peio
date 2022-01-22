#pragma once

#include "IOSocket.h"
#include "BindSocket.h"
#include "ListenSocket.h"
#include "AcceptSocket.h"
#include "ConnectSocket.h"
#include "CloseSocket.h"

namespace Peio::Net {

	using ListenerSocket = PEIO_NET_EXPORT Socket<ListenSocket, BindSocket>;
	using ServerSocket = PEIO_NET_EXPORT Socket<CloseSocket, SendSocket, ReceiveSocket, AcceptSocket>;
	using ClientSocket = PEIO_NET_EXPORT Socket<CloseSocket, SendSocket, ReceiveSocket, ConnectSocket, BindSocket>;

}