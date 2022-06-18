#pragma once

#include "GUIHeader.h"
#include "..\Windows\Input.h"

#include <map>

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT LayerHandlerSet : public virtual EventHandler<Win::WinEvent> {
	
		bool Handle(Win::WinEvent* event) override;
		
		_NODISCARD BaseHandlerSet<Win::WinEvent>& GetLayer(size_t index);
		_NODISCARD BaseHandlerSet<Win::WinEvent>& operator[](size_t index);
	
	protected:
	
		std::map<size_t, BaseHandlerSet<Win::WinEvent>> layers = {};
	
	};

}