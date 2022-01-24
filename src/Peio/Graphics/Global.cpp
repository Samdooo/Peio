#define PEIO_GFX_EXPORTING
#include "Global.h"

Microsoft::WRL::ComPtr<ID3D12Device> Peio::Gfx::device = nullptr;

void Peio::Gfx::Init()
{
	HRESULT ret = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), &device);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create ID3D12Device.", ret);
	}
}
