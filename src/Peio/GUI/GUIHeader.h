#pragma once

#ifdef PEIO_GUI_EXPORTING
#define PEIO_GUI_EXPORT __declspec(dllexport)
#else
#define PEIO_GUI_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"
#include "..\Graphics\GraphicsHeader.h"
#include "..\Media\MediaHeader.h"

#pragma comment(lib, "Peio-Graphics.lib")
#pragma comment(lib, "Peio-Media.lib")