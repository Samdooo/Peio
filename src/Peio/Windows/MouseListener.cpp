#define PEIO_WIN_EXPORTING
#include "MouseListener.h"

namespace Peio::Win {

	Procedure<WinEvent*> mouseListener = Listener([](WinMessageEvent* event)
	{
		Int2 pos = { GET_X_LPARAM(event->msg.lParam), GET_Y_LPARAM(event->msg.lParam) };
		if (event->msg.message == WM_MOUSEWHEEL)
			ScreenToClient(event->msg.hwnd, (LPPOINT)&pos);

		switch (event->msg.message) {
		case WM_MOUSEMOVE:
			Input::HandleNewEvent(MouseMoveEvent{ event->msg, pos });
			break;
		case WM_LBUTTONDOWN:
			Input::HandleNewEvent(MouseButtonDownEvent{ event->msg, MouseButton::LEFT, pos });
			break;
		case WM_LBUTTONUP:
			Input::HandleNewEvent(MouseButtonUpEvent{ event->msg, MouseButton::LEFT, pos });
			break;
		case WM_RBUTTONDOWN:
			Input::HandleNewEvent(MouseButtonDownEvent{ event->msg, MouseButton::RIGHT, pos });
			break;
		case WM_RBUTTONUP:
			Input::HandleNewEvent(MouseButtonUpEvent{ event->msg, MouseButton::RIGHT, pos });
			break;
		case WM_MBUTTONDOWN:
			Input::HandleNewEvent(MouseButtonDownEvent{ event->msg, MouseButton::MIDDLE, pos });
			break;
		case WM_MBUTTONUP:
			Input::HandleNewEvent(MouseButtonUpEvent{ event->msg, MouseButton::MIDDLE, pos });
			break;
		case WM_XBUTTONDOWN:
			Input::HandleNewEvent(MouseButtonDownEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos });
			break;
		case WM_XBUTTONUP:
			Input::HandleNewEvent(MouseButtonUpEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos });
			break;
		case WM_MOUSEWHEEL:
			Input::HandleNewEvent(MouseWheelEvent{ event->msg, GET_WHEEL_DELTA_WPARAM(event->msg.wParam), pos });
			break;
		default:
			break;
		}
		return false;
	});

}