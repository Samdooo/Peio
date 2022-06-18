#pragma once

#include "Rectangle.h"
#include "..\Windows\Input.h"
#include "..\Windows\MouseListener.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Button : public virtual Rectangle, public EventHandler<Win::MouseMoveEvent, 
		Win::MouseButtonDownEvent, Win::MouseButtonUpEvent, Win::MouseWheelEvent> {
	
		Button();
		~Button();
	
	protected:
	
		virtual void OnMouseMove(Win::MouseMoveEvent*) {}
		virtual void OnMouseEnter(Win::MouseMoveEvent*) {}
		virtual void OnMouseLeave(Win::MouseMoveEvent*) {}
		virtual void OnMouseDown(Win::MouseButtonDownEvent*) {}
		virtual void OnMouseUp(Win::MouseButtonUpEvent*) {}
		virtual void OnMouseWheel(Win::MouseWheelEvent*) {}
	
		bool wasHovered = false;
		bool IsHovered(Int2 pos);
	
		bool Handle(Win::MouseMoveEvent* event) override;
		bool Handle(Win::MouseButtonDownEvent* event) override;
		bool Handle(Win::MouseButtonUpEvent* event) override;
		bool Handle(Win::MouseWheelEvent* event) override;
	
	};

}