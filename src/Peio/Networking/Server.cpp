#define PEIO_NET_EXPORTING
#include "Server.h"

namespace Peio::Net {

	void Server::Init(EventHandler<>* handler, int bufferLength, ushort port, int type, int af)
	{
		this->handler = handler;
		listener.Init(type, af);

		Peio::Net::Hint local;
		local.Init(port);
		listener.Bind(local);

		this->bufferLength = bufferLength;
		buffer = new char[bufferLength];
	}

	void Server::Listen(int backlog)
	{
		listener.Listen(backlog);
		listener.RegisterEvents();
	}

	void Server::Update(DWORD wait)
	{
		listener.Update(this, wait);
		for (auto& client : clients)
			client.second->Update(this, wait);
	}

	void Server::Close()
	{
		if (listener.GetSocket())
			listener.Close();
		for (auto& client : clients)
			if (client.second->GetSocket())
				client.second->Close();
		PEIO_SAFE_DELETEALL(buffer);
	}

	Server::~Server()
	{
		Close();
	}

	void Server::Handle(AcceptEvent& event)
	{
		ServerSocket* client = new ServerSocket;
		client->Accept(event.sock->GetSocket());
		client->RegisterEvents();
		clients.insert({ client->GetSocket(), client });
		ServerConnectionEvent connectionEvent = { client };
		handler->Handle(connectionEvent);
	}

	void Server::Handle(ReceiveEvent& event)
	{
		if (!clients.count(event.sock->GetSocket()))
			return;
		ServerSocket* client = clients.at(event.sock->GetSocket());
		int length = client->Receive(buffer, bufferLength);
		ServerReceiveEvent receiveEvent = { client, buffer, length };
		handler->Handle(receiveEvent);
	}

	void Server::Handle(SendEvent& event)
	{
	}

	void Server::Handle(CloseEvent& event)
	{
		if (!clients.count(event.sock->GetSocket()))
			return;
		ServerDisconnectionEvent disconnectionEvent = { clients.at(event.sock->GetSocket()) };
		handler->Handle(disconnectionEvent);
		delete clients.at(event.sock->GetSocket());
		clients.erase(event.sock->GetSocket());
	}

}