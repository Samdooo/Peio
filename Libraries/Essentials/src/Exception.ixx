export module Peio.Essentials.Exception;

export import <string>;

export namespace Peio {

	namespace Essentials {

		struct Exception {

			const std::string message;

			Exception(const std::string& message) : message(message) {}

		};

	}
	using namespace Essentials;

}