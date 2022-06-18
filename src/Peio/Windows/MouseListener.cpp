#define PEIO_WIN_EXPORTING
#include "MouseListener.h"

namespace Peio::Win {

	bool MouseListener::Handle(WinMessageEvent* event)
	{
		Int2 pos = { GET_X_LPARAM(event->msg.lParam), GET_Y_LPARAM(event->msg.lParam) };

		switch (event->msg.message) {
		case WM_MOUSEMOVE:
			Input::eventHandlers.HandleNew(MouseMoveEvent{ event->msg, pos });
			break;
		case WM_LBUTTONDOWN:
			Input::eventHandlers.HandleNew(MouseButtonDownEvent{ event->msg, MouseButton::LEFT, pos });
			break;
		case WM_LBUTTONUP:
			Input::eventHandlers.HandleNew(MouseButtonUpEvent{ event->msg, MouseButton::LEFT, pos });
			break;
		case WM_RBUTTONDOWN:
			Input::eventHandlers.HandleNew(MouseButtonDownEvent{ event->msg, MouseButton::RIGHT, pos });
			break;
		case WM_RBUTTONUP:
			Input::eventHandlers.HandleNew(MouseButtonUpEvent{ event->msg, MouseButton::RIGHT, pos });
			break;
		case WM_MBUTTONDOWN:
			Input::eventHandlers.HandleNew(MouseButtonDownEvent{ event->msg, MouseButton::MIDDLE, pos });
			break;
		case WM_MBUTTONUP:
			Input::eventHandlers.HandleNew(MouseButtonUpEvent{ event->msg, MouseButton::MIDDLE, pos });
			break;
		case WM_XBUTTONDOWN:
			Input::eventHandlers.HandleNew(MouseButtonDownEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos });
			break;
		case WM_XBUTTONUP:
			Input::eventHandlers.HandleNew(MouseButtonUpEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos });
			break;
		case WM_MOUSEWHEEL:
			Input::eventHandlers.HandleNew(MouseWheelEvent{ event->msg, GET_WHEEL_DELTA_WPARAM(event->msg.wParam), pos });
			break;
		default:
			break;
		}
		return false;
	}

}