#define PEIO_GFX_EXPORTING
#include "SwapChain.h"

void Peio::Gfx::SwapChain::Init(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen)
{
	HRESULT ret;
	Release();

	ret = CreateDXGIFactory1(__uuidof(IDXGIFactory2), &factory);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create factory.", ret);
	}
	
	//Resize(cmdQueue, hwnd, size, numBuffers, fullscreen);
}

void Peio::Gfx::SwapChain::Resize(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, UINT numBuffers, bool fullscreen)
{
	HRESULT ret;

	DXGI_SWAP_CHAIN_DESC1 chainDesc = {};
	chainDesc = {};
	chainDesc.Width = size.x();
	chainDesc.Height = size.y();
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.Stereo = FALSE;
	chainDesc.SampleDesc = { 1, 0 };
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = numBuffers;
	chainDesc.Scaling = DXGI_SCALING_NONE;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	chainDesc.Flags = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsChainDesc = {};
	fsChainDesc.Windowed = !fullscreen;
	fsChainDesc.RefreshRate = { 0, 0 };

	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChainTmp = nullptr;
	ret = factory->CreateSwapChainForHwnd(cmdQueue, hwnd, &chainDesc, &fsChainDesc, nullptr, &swapChainTmp);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create swap chain.", ret);
	}

	if (swapChain) {
		swapChain.ReleaseAndGetAddressOf();
		swapChain = nullptr;
	}

	ret = swapChainTmp->QueryInterface(__uuidof(IDXGISwapChain3), &swapChain);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to convert swap chain.", ret);
	}
	swapChainTmp.ReleaseAndGetAddressOf();

	if (fullscreen)
		swapChain->SetFullscreenState(TRUE, nullptr);
}

void Peio::Gfx::SwapChain::SetFullscreenState(BOOL fullscreen)
{
	swapChain->SetFullscreenState(fullscreen, nullptr);
}

void Peio::Gfx::SwapChain::GetBuffer(UINT buffer, ID3D12Resource** renderTarget)
{
	HRESULT ret = swapChain->GetBuffer(buffer, __uuidof(ID3D12Resource), reinterpret_cast<void**>(renderTarget));
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to get buffer.", ret);
	}
}

UINT Peio::Gfx::SwapChain::SwapBuffers()
{
	HRESULT ret = swapChain->Present(0, 0);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to present buffer.", ret);
	}
	return swapChain->GetCurrentBackBufferIndex();
}

void Peio::Gfx::SwapChain::Release()
{
	if (swapChain) {
		swapChain->SetFullscreenState(FALSE, nullptr); /* To avoid getting weird errors */
		swapChain.ReleaseAndGetAddressOf();
		swapChain = nullptr;
	}
	if (factory) {
		factory.ReleaseAndGetAddressOf();
		factory = nullptr;
	}
}

Peio::Gfx::SwapChain::~SwapChain()
{
	Release();
}
