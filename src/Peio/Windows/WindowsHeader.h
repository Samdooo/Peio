#pragma once

#ifdef PEIO_WIN_EXPORTING
#define PEIO_WIN_EXPORT __declspec(dllexport)
#else
#define PEIO_WIN_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"
#pragma comment(lib, "Peio-Essentials.lib")

#define NOMINMAX

#include <Windows.h>
#include <windowsx.h>
