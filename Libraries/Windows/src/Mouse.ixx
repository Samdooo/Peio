export module Peio.Windows.Mouse;

export import Peio.Essentials.Types;
export import Peio.Essentials.Vec;
export import Peio.Windows.Input;
export import Peio.Windows.Window;
export import Peio.Windows.WindowsException;

import <Windows.h>;

export namespace Peio {

	namespace Windows {

		enum class MouseButton { Left, Right, Middle, X1, X2 };
		enum class MouseAdditional { None = 0, Control = MK_CONTROL, Left = MK_LBUTTON, Right = MK_RBUTTON, 
									 Middle = MK_MBUTTON, Shift = MK_SHIFT, X1 = MK_XBUTTON1, X2 = MK_XBUTTON2 };

		struct MouseEvent : public WindowsEvent {
			Short2 pos {};
			MouseAdditional additional = MouseAdditional::None;
		};
		struct MouseButtonEvent : public MouseEvent {
			MouseButton button {};
		};

		struct MouseMoveEvent : public MouseEvent {};
		struct MouseDownEvent : public MouseButtonEvent {};
		struct MouseUpEvent : public MouseButtonEvent {};

		struct MouseWheelEvent : public MouseEvent {
			short delta = 0;
		};

		MessageHandler CreateMouseMoveHandler(Proc<MouseMoveEvent&> onMouseMove);
		MessageHandler CreateMouseDownHandler(Proc<MouseDownEvent&> onMouseDown);
		MessageHandler CreateMouseUpHandler(Proc<MouseUpEvent&> onMouseUp);
		MessageHandler CreateMouseWheelHandler(Proc<MouseWheelEvent&> onMouseWheel);

		struct RawMouseEvent : public WindowsEvent {
			bool foreground = false;
		};
		struct RawMouseMoveEvent : public RawMouseEvent {
			Int2 movement {};
		};
		struct RawMouseButtonEvent : public RawMouseEvent {
			MouseButton button {};
		};
		struct RawMouseDownEvent : public RawMouseButtonEvent {};
		struct RawMouseUpEvent : public RawMouseButtonEvent {};

		struct RawMouseWheelEvent : public RawMouseEvent {
			short delta = 0;
		};

		struct FailedRawMouseInputRegistrationException : public WindowsException {
			FailedRawMouseInputRegistrationException() : WindowsException("Failed to register raw mouse input.") {}
		};

		void RegisterRawMouseInput(const Window& window);

		MessageHandler CreateRawMouseHandler(Proc<RawMouseMoveEvent&> onRawMouseMove, 
			Proc<RawMouseDownEvent&> onRawMouseDown, Proc<RawMouseUpEvent&> onRawMouseUp,
			Proc<RawMouseWheelEvent&> onRawMouseWheel);

	}
	using namespace Windows;

}