#pragma once

#ifdef PEIO_GFX_EXPORTING
#define PEIO_GFX_EXPORT __declspec(dllexport)
#else
#define PEIO_GFX_EXPORT __declspec(dllimport)
#endif

#include "..\PeioHeader.h"

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <wrl.h>

#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")