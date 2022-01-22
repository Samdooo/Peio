#pragma once

#ifdef PEIO_WIN_EXPORTING
#define PEIO_WIN_EXPORT __declspec(dllexport)
#else
#define PEIO_WIN_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"

#define NOMINMAX

#include <Windows.h>
#include <windowsx.h>