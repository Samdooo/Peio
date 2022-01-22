#include <iostream>

#include <Peio/Networking/Init.h>
#include <Peio/Networking/Sockets.h>

struct NetHandler : public Peio::EventHandler<Peio::Net::AcceptEvent, Peio::Net::ReceiveEvent> {



};

int main() {
	
	try {

		Peio::Net::Init();

		Peio::Net::ListenerSocket listener;
		listener.Init();

		Peio::Net::Hint hint;
		hint.Init(14000);
		listener.Bind(hint);

		listener.RegisterEvents();

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