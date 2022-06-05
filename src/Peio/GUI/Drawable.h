#pragma once

#include "GUIHeader.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Drawable {

		virtual void Draw() = 0;

	};

}