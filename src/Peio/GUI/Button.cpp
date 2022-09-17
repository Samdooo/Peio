#define PEIO_GUI_EXPORTING
#include "Button.h"

namespace Peio::GUI {

	Button::Button()
	{
		//Win::Input::AddEventHandler(static_cast<EventHandler<Win::MouseMoveEvent, 
		//	Win::MouseButtonDownEvent, Win::MouseButtonUpEvent, Win::MouseWheelEvent>*>(this));
	}
	
	Button::~Button()
	{
		//Win::Input::RemoveEventHandler(static_cast<EventHandler<Win::MouseMoveEvent, 
		//	Win::MouseButtonDownEvent, Win::MouseButtonUpEvent, Win::MouseWheelEvent>*>(this));
	}
	
	bool Button::IsHovered(Int2 pos)
	{
		Int2 topLeft = (Int2)Rectangle::position.GetTranslated();
		Int2 bottomRight = topLeft + (Int2)Rectangle::size;
		return (pos.x() >= topLeft.x() && pos.y() >= topLeft.y() && pos.x() < bottomRight.x() && pos.y() < bottomRight.y());
	}
	
	bool Button::Handle(Win::MouseMoveEvent* event)
	{
		bool hovered = IsHovered(event->position);
		if (hovered)
			OnMouseMove(event);
		if (hovered && !wasHovered)
			OnMouseEnter(event);
		else if (!hovered && wasHovered)
			OnMouseLeave(event);
	
		wasHovered = hovered;
		return hovered;
	}
	
	bool Button::Handle(Win::MouseButtonDownEvent* event)
	{
		bool hovered = IsHovered(event->position);
		if (hovered) {
			OnMouseDown(event);
		}
		return hovered;
	}
	
	bool Button::Handle(Win::MouseButtonUpEvent* event)
	{
		bool hovered = IsHovered(event->position);
		if (hovered) {
			OnMouseUp(event);
		}
		return hovered;
	}

	bool Button::Handle(Win::MouseWheelEvent* event)
	{
		bool hovered = IsHovered(event->position);
		if (hovered) {
			OnMouseWheel(event);
		}
		return hovered;
	}

}