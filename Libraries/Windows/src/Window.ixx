export module Peio.Windows.Window;

import <Windows.h>;
export import <string>;
export import Peio.Windows.WindowsException;
export import Peio.Essentials.Vec;

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

		void CreateClass(std::string className, UINT classStyles = 0, WNDPROC proc = DefWindowProc);

		class Window {

		public:

			void Create(std::string className, std::string title, Int2 size, DWORD styles = WS_OVERLAPPEDWINDOW, Int2 position = { CW_USEDEFAULT, CW_USEDEFAULT }, DWORD exStyles = 0);
			void HandleMessages();
			void Close();

			void Show();
			void Hide();
			bool IsOpen();

		protected:

			HWND hwnd;

		};

	}
	using namespace Windows;

}