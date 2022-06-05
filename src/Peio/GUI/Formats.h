#pragma once

#include "GUIHeader.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Formats {

		static DXGI_FORMAT ToDXGIFormat(AVPixelFormat format);
		static AVPixelFormat ToAVPixelFormat(DXGI_FORMAT format);

	};

}