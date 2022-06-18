#pragma once

#ifdef PEIO_GFX_EXPORTING
#define PEIO_GFX_EXPORT __declspec(dllexport)
#else
#define PEIO_GFX_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"
#pragma comment(lib, "Peio-Essentials.lib")

#include "..\Array.h"
#include "..\Exception.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <wrl.h>

#undef min
#undef max

#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")