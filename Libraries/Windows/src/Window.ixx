export module Peio.Windows.Window;

import <Windows.h>;
export import <string>;
export import Peio.Windows.WindowsException;
export import Peio.Essentials.Vec;

import Peio.Windows.Input;

export namespace Peio {

	namespace Windows {

		struct FailedClassRegistrationException : public WindowsException {
			FailedClassRegistrationException() : WindowsException("Failed to register window class.") {}
		};
		struct FailedWindowCreationException : public WindowsException {
			FailedWindowCreationException() : WindowsException("Failed to create window.") {}
		};
		struct FailedWindowClosureException : public WindowsException {
			FailedWindowClosureException() : WindowsException("Failed to close window.") {}
		};

		void CreateClass(std::string className, UINT classStyles = 0, WNDPROC proc = WndProc);

		class Window {

		public:

			void Create(std::string className, std::string title, Int2 size, DWORD styles = WS_OVERLAPPEDWINDOW, Int2 position = { CW_USEDEFAULT, CW_USEDEFAULT }, DWORD exStyles = 0);
			void HandleMessages() const;
			void Close() const;

			void Show() const;
			void Hide() const;
			bool IsOpen() const;

			HWND GetHWND() const;

		protected:

			HWND hwnd;

		};

	}
	using namespace Windows;

}