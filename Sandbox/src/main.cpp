#include <iostream>
#include <Peio/Exception.h>

#include <Peio/EventHandler.h>

#include <unordered_map>

struct Handlers {

	std::unordered_multimap<size_t, Peio::EventHandler<>*> handlers;

	template <typename... T_events>
	void AddHandler(Peio::EventHandler<T_events...>* handler) {
		(handlers.insert(std::pair(typeid(T_events).hash_code(), static_cast<Peio::EventHandler<T_events>*>(handler))), ...);
	}

	template <typename T_event>
	void Handle(T_event& event) {
		auto its = handlers.equal_range(typeid(T_event).hash_code());
		for (auto it = its.first; it != its.second; it++) {
			std::cout << "Test" << std::endl;
			it->second->Handle(event);
		}
	}

};

struct TestHandler : public Peio::EventHandler<int, char> {

	void Handle(int& i) override {
		std::cout << "Int: " << i << std::endl;
	}

	void Handle(char& i) override {
		std::cout << "Char: " << i << std::endl;
	}

};

int main() {

	try {

		Handlers handlers;
		TestHandler handler;
		TestHandler handler2;
		handlers.AddHandler<int, char>(&handler);
		handlers.AddHandler<int, char>(&handler2);
		
		int i = 420;
		handlers.Handle(i);

		char c = 69;
		handlers.Handle(c);

	}
	catch (Peio::Exception exception) {
		std::cout << "Exception: " << exception.msg << " " << exception.file
			<< " at line " << exception.line << "." << std::endl;
	}

	return 0;
}