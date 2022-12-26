#pragma once

#include "Socket.h"

namespace Peio::Net {

	template <template <typename> typename T_sock>
	struct SocketEvent {
		T_sock<Socket<>>* sock;
		int errorCode;
	};

	template <typename T_sock, template <typename> typename T_super, long L_event>
	struct EventSocket : public T_sock {

		static constexpr long events = (1 << L_event);

		void Update(EventHandler<>* handler, WSANETWORKEVENTS& netEvents) {
			if (netEvents.lNetworkEvents & (1 << L_event)) {
				SocketEvent<T_super> e{ reinterpret_cast<T_super<Socket<>>*>(this), netEvents.iErrorCode[L_event] };
				handler->Handle(&e);
			}
		}

	};

}