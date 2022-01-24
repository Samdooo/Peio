#pragma once

#ifdef PEIO_VXL_EXPORTING
#define PEIO_VXL_EXPORT __declspec(dllexport)
#else
#define PEIO_VXL_EXPORT __declspec(dllimport)
#endif

#include "..\Graphics\GraphicsHeader.h"

#ifdef _DEBUG
#pragma comment(lib, "Peio-Graphics-debug.lib")
#else
#pragma comment(lib, "Peio-Graphics.lib")
#endif