#pragma once

#include "Global.h"
#include "ShaderResourceView.h"
#include "UnorderedAccessView.h"
#include "RootParameter.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT RootSignature {

		std::vector<ShaderResourceView> srvs = {};
		std::vector<UnorderedAccessView> uavs = {};
		std::vector<D3D12_STATIC_SAMPLER_DESC> samplers = {};

		void Init(D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
		void SetRootSignature(ID3D12GraphicsCommandList* cmdList);

		_NODISCARD ID3D12RootSignature* GetRootSignature() const noexcept;

	protected:

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	};

	//struct PEIO_GFX_EXPORT RootSignature {
	//
	//	static ID3D12RootSignature* Create(const std::vector<D3D12_ROOT_PARAMETER>& params
	//		, const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers = std::vector<D3D12_STATIC_SAMPLER_DESC>()
	//		, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
	//
	//};

}