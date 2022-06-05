#pragma once

#include "FileDecoder.h"

namespace Peio::Med {

	struct PEIO_MED_EXPORT Images {

		static Frame Load(std::string path, AVPixelFormat targetFormat = AV_PIX_FMT_NONE, Int2 targetSize = {}, int algorithm = SWS_BICUBIC);

	};

}