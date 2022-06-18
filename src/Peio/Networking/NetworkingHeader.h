#pragma once

#ifdef PEIO_NET_EXPORTING
#define PEIO_NET_EXPORT __declspec(dllexport)
#else
#define PEIO_NET_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"
#pragma comment(lib, "Peio-Essentials.lib")

#define NOMINMAX

#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "ws2_32.lib")