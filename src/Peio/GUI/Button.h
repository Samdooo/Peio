#pragma once

#include "Rectangle.h"
#include "..\Windows\Input.h"
#include "..\Windows\MouseListener.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Button : public virtual Rectangle, protected EventHandler<Win::MouseMoveEvent, 
		Win::MouseButtonDownEvent, Win::MouseButtonUpEvent, Win::MouseWheelEvent> {

		Button();
		~Button();

		bool IsHovered();

	protected:

		virtual void OnMouseMove(Win::MouseMoveEvent&) {}
		virtual void OnMouseEnter(Win::MouseMoveEvent&) {}
		virtual void OnMouseLeave(Win::MouseMoveEvent&) {}
		virtual void OnMouseDown(Win::MouseButtonDownEvent&) {}
		virtual void OnMouseUp(Win::MouseButtonUpEvent&) {}
		virtual void OnMouseWheel(Win::MouseWheelEvent&) {}

		bool wasHovered = false;
		bool IsHovered(Int2 pos);

		void Handle(Win::MouseMoveEvent& event) override;
		void Handle(Win::MouseButtonDownEvent& event) override;
		void Handle(Win::MouseButtonUpEvent& event) override;
		void Handle(Win::MouseWheelEvent& event) override;

	};

}