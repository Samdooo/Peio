#define PEIO_NET_EXPORTING
#include "Sockets.h"

namespace Peio::Net {

	template struct Peio::Net::ListenerSocket;
	template struct Peio::Net::ServerSocket;
	template struct Peio::Net::ClientSocket;

}