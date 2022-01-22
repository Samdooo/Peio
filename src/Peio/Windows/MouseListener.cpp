#define PEIO_WIN_EXPORTING
#include "MouseListener.h"

namespace Peio::Win {

	void MouseListener::Handle(Input::Message& msg)
	{
		Int2 pos = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };

		switch (msg.message) {
		case WM_MOUSEMOVE:
			Input::HandleNew(MouseMoveEvent{ pos, msg.wParam });
			break;
		case WM_LBUTTONDOWN:
			Input::HandleNew(MouseButtonDownEvent{ MouseButton::LEFT, pos, msg.wParam });
			break;
		case WM_LBUTTONUP:
			Input::HandleNew(MouseButtonUpEvent{ MouseButton::LEFT, pos, msg.wParam });
			break;
		case WM_RBUTTONDOWN:
			Input::HandleNew(MouseButtonDownEvent{ MouseButton::RIGHT, pos, msg.wParam });
			break;
		case WM_RBUTTONUP:
			Input::HandleNew(MouseButtonUpEvent{ MouseButton::RIGHT, pos, msg.wParam });
			break;
		case WM_MBUTTONDOWN:
			Input::HandleNew(MouseButtonDownEvent{ MouseButton::MIDDLE, pos, msg.wParam });
			break;
		case WM_MBUTTONUP:
			Input::HandleNew(MouseButtonUpEvent{ MouseButton::MIDDLE, pos, msg.wParam });
			break;
		case WM_XBUTTONDOWN:
			Input::HandleNew(MouseButtonDownEvent{ HIWORD(msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos, msg.wParam });
			break;
		case WM_XBUTTONUP:
			Input::HandleNew(MouseButtonUpEvent{ HIWORD(msg.wParam) == XBUTTON1 ? MouseButton::X1 : MouseButton::X2, pos, msg.wParam });
			break;
		case WM_MOUSEWHEEL:
			Input::HandleNew(MouseWheelEvent{ GET_WHEEL_DELTA_WPARAM(msg.wParam), pos, msg.wParam });
			break;
		default:
			break;
		}
	}

}