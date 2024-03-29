#pragma once

#include "Sockets.h"
#include <unordered_map>

namespace Peio::Net {

	struct ServerConnectionEvent {
		ServerSocket* client;
	};
	struct ServerReceiveEvent {
		ServerSocket* client;
		char* buffer;
		int length;
	};
	struct ServerDisconnectionEvent {
		ServerSocket* client;
	};

	struct PEIO_NET_EXPORT Server : protected EventHandler<AcceptEvent, ReceiveEvent, SendEvent, CloseEvent> {

		void Init(EventHandler<>* handler, int bufferLength, ushort port, int type = SOCK_STREAM, int af = AF_INET);
		void Listen(int backlog = SOMAXCONN);
		
		void Update(DWORD wait = 0);

		void Close();

		~Server();

	protected:

		bool Handle(AcceptEvent* event) override;
		bool Handle(ReceiveEvent* event) override;
		bool Handle(SendEvent* event) override;
		bool Handle(CloseEvent* event) override;

		EventHandler<>* handler = nullptr;
		ListenerSocket listener = {};
		std::unordered_map<SOCKET, ServerSocket*> clients = {};
		char* buffer = nullptr;
		int bufferLength = 0;

	};

}