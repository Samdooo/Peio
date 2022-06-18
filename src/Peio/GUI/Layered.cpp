#define PEIO_GUI_EXPORTING
#include "Layered.h"

namespace Peio::GUI {

	bool LayerHandlerSet::Handle(Win::WinEvent* event)
	{
		for (auto& layer : layers) {
			if (layer.second.Handle(event))
				return true;
		}
		return false;
	}
	
	BaseHandlerSet<Win::WinEvent>& LayerHandlerSet::GetLayer(size_t index)
	{
		return layers[index];
	}
	
	BaseHandlerSet<Win::WinEvent>& LayerHandlerSet::operator[](size_t index)
	{
		return GetLayer(index);
	}

}