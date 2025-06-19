export module Peio.Windows.WindowsException;

import <Windows.h>;
export import Peio.Essentials.Exception;

export namespace Peio {

	namespace Windows {

		struct WindowsException : public Exception {

			const DWORD errorCode;
			WindowsException(const std::string& message) : Exception(message), errorCode(GetLastError()) {}

		};

	}
	using namespace Windows;

}