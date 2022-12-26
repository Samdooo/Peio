#pragma once

#include "Exception.h"
#include "..\EventHandler.h"

namespace Peio::Net {

	template <template <typename> typename... T_socks>
	struct Socket {

		void Init(int type = SOCK_STREAM, int af = AF_INET) {
			if (sock)
				Close();
			sock = socket(af, type, 0);
			if (sock == INVALID_SOCKET) {
				throw PEIO_NET_EXCEPTION("Failed to create socket.");
			}
		}

		void Close() {
			if (closesocket(sock) != 0) {
				sock = 0;
				throw PEIO_NET_EXCEPTION("Failed to close socket.");
			}
			sock = 0;
		}

		void SetSocket(SOCKET sock) noexcept {
			this->sock = sock;
		}

		_NODISCARD SOCKET GetSocket() const noexcept {
			return sock;
		}

	protected:

		static constexpr long events = 0;
		SOCKET sock = 0;
		HANDLE event = nullptr;

	};

	//Recursive hierarchical inheritance
	template <template <typename> typename T_sock, template <typename> typename... T_socks>
	struct Socket<T_sock, T_socks...> : public T_sock<Socket<T_socks...>> {
		static_assert(std::is_base_of<Socket<T_socks...>, T_sock<Socket<T_socks...>>>::value, "T_sock must inherit its template parameter.");

		void RegisterEvents() {
			if (Socket<>::event) {
				WSACloseEvent(Socket<>::event);
			}
			Socket<>::event = WSACreateEvent();
			if (Socket<>::event == WSA_INVALID_EVENT) {
				throw PEIO_NET_EXCEPTION("Failed to create events.");
			}
			if (WSAEventSelect(Socket<>::sock, Socket<>::event, events) != 0) {
				throw PEIO_NET_EXCEPTION("Failed to register events.");
			}
		}

		void Update(EventHandler<>* handler, DWORD wait = 0) {
			DWORD ret = WaitForSingleObject(Socket<>::event, wait);
			if (ret == WAIT_OBJECT_0) {
				WSANETWORKEVENTS netEvents = {};
				if (WSAEnumNetworkEvents(Socket<>::sock, Socket<>::event, &netEvents) != 0) {
					throw PEIO_NET_EXCEPTION("Failed to get event info.");
				}
				Update(handler, netEvents);
			}
			else if (ret == WAIT_FAILED) {
				throw PEIO_NET_EXCEPTION("Failed to get event status.");
			}
		}

		using _T_sock = T_sock<Socket<T_socks...>>;
		static constexpr long events = _T_sock::events | Socket<T_socks...>::events;

		void Update(EventHandler<>* handler, WSANETWORKEVENTS& netEvents) {
			if constexpr ((bool)sizeof...(T_socks)) {
				Socket<T_socks...>::Update(handler, netEvents);
				//Update might not be overridden
				if constexpr (&Socket<T_socks...>::Update != &_T_sock::Update) {
					_T_sock::Update(handler, netEvents);
				}
			}
		}

	};

}