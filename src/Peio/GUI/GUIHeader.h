#pragma once

#ifdef PEIO_GUI_EXPORTING
#define PEIO_GUI_EXPORT __declspec(dllexport)
#else
#define PEIO_GUI_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"
#include "..\Graphics\GraphicsHeader.h"
#include "..\Media\MediaHeader.h"
#include "..\Windows\WindowsHeader.h"

#pragma comment(lib, "Peio-Graphics.lib")
#pragma comment(lib, "Peio-Media.lib")
#pragma comment(lib, "Peio-Windows.lib")

extern "C" {
#include "FreeType/ft2build.h"
#include "FreeType/freetype/freetype.h"
}
#pragma comment(lib, "FreeType/freetype.lib")