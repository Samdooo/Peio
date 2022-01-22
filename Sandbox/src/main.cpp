#include <iostream>

#include <Peio/Networking/Init.h>
#include <Peio/Networking/Sockets.h>

#include <unordered_map>

struct NetHandler : public Peio::EventHandler<Peio::Net::AcceptEvent, Peio::Net::ReceiveEvent> {

	std::unordered_map<SOCKET, Peio::Net::ServerSocket*> clients = {};

	void Handle(Peio::Net::AcceptEvent& event) override {
		std::cout << event.sock->GetSocket() << " connected" << std::endl;
		Peio::Net::ServerSocket* client = new Peio::Net::ServerSocket;
		client->Accept(event.sock->GetSocket());
		client->RegisterEvents();
		clients.insert(std::pair(client->GetSocket(), client));
	}

	char* buf = new char[4096];

	void Handle(Peio::Net::ReceiveEvent& event) override {
		int length = event.sock->Receive(buf, 4096);
		std::cout << "Received from " << event.sock << ": " << std::string(buf, length) << std::endl;
	}

};

int main() {
	
	try {

		Peio::Net::Init();

		Peio::Net::ListenerSocket listener;
		listener.Init();

		Peio::Net::Hint hint;
		hint.Init(14000);
		listener.Bind(hint);
		listener.Listen();
		listener.RegisterEvents();

		NetHandler handler;

		while (true) {
			Sleep(1000);
			listener.Update(&handler);
			for (auto& client : handler.clients)
				client.second->Update(&handler);
			std::cout << "Updated" << std::endl;
		}

		WSACleanup();

	}
	catch (Peio::Net::Exception exception) {
		std::cout << "Net exception: " << exception.msg << " in " << exception.file
			<< " at line " << exception.line << ". WSAError: " << exception.wsaError << std::endl;
	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}