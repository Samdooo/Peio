#pragma once

#include "MediaHeader.h"
#include "..\Exception.h"
#include "Exception.h"
#include <string>

namespace Peio::Med {

	PEIO_MED_EXPORT void Remux(std::string inPath, std::string outPath);

}