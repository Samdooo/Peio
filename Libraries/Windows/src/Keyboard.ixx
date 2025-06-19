export module Peio.Windows.Keyboard;

export import Peio.Essentials.Types;
export import Peio.Windows.Input;
export import Peio.Windows.Window;
export import Peio.Windows.WindowsException;

export namespace Peio {

	namespace Windows {

		struct KeyEvent : public WindowsEvent {
			uchar key;
			ushort repeatCount;
			bool prevState;
		};
		struct KeyDownEvent : public KeyEvent {};
		struct KeyUpEvent : public KeyEvent {};
		struct CharEvent : public KeyEvent {};

		MessageHandler CreateKeyDownHandler(Proc<KeyDownEvent&> onKeyDown);
		MessageHandler CreateKeyUpHandler(Proc<KeyUpEvent&> onKeyUp);
		MessageHandler CreateCharHandler(Proc<CharEvent&> onChar);

		struct FailedRawKeyboardInputRegistrationException : public WindowsException {
			FailedRawKeyboardInputRegistrationException() : WindowsException("Failed to register raw keyboard input.") {}
		};

		void RegisterRawKeyboardInput(const Window& window);

		struct RawKeyEvent : public WindowsEvent {
			bool foreground;
			ushort key;
			ushort scanCode;
			UINT winMessage;
			bool invalid;
		};
		struct RawKeyDownEvent : public RawKeyEvent {};
		struct RawKeyUpEvent : public RawKeyEvent {};

		MessageHandler CreateRawKeyHandler(Proc<RawKeyDownEvent&> onRawKeyDown, Proc<RawKeyUpEvent&> onRawKeyUp);

	}
	using namespace Windows;

}