#pragma once

#include "GUIHeader.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Uploadable {

		virtual void Upload() = 0;

	};

}