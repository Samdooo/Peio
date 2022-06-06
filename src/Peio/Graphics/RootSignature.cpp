#define PEIO_GFX_EXPORTING
#include "RootSignature.h"

namespace Peio::Gfx {

	ID3D12RootSignature* RootSignature::Create(const std::vector<D3D12_ROOT_PARAMETER>& params, const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers, D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		HRESULT ret;

		ID3D12RootSignature* rootSignature = nullptr;

		//D3D12_ROOT_PARAMETER* pParams = nullptr;
		//if (params.size()) {
		//	pParams = new D3D12_ROOT_PARAMETER[params.size()];
		//	memcpy(pParams, &params[0], sizeof(D3D12_ROOT_PARAMETER) * params.size());
		//}
		//
		//D3D12_STATIC_SAMPLER_DESC* pSamplers = nullptr;
		//if (samplers.size()) {
		//	pSamplers = new D3D12_STATIC_SAMPLER_DESC[samplers.size()];
		//	memcpy(pSamplers, &samplers[0], sizeof(D3D12_STATIC_SAMPLER_DESC) * samplers.size());
		//}

		CD3DX12_ROOT_SIGNATURE_DESC desc;
		desc.Init((UINT)params.size(), &params[0], (UINT)samplers.size(), &samplers[0], flags);

		Microsoft::WRL::ComPtr<ID3DBlob> signature = nullptr;
		//ID3DBlob* signature = nullptr;
		ret = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to serialize root signature.", ret);
		}

		ret = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature), reinterpret_cast<void**>(&rootSignature));
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create root signature.", ret);
		}

		for (size_t i = 0; i < params.size(); i++) {
			if (params[i].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
				delete[] params[i].DescriptorTable.pDescriptorRanges;
		}

		return rootSignature;
	}

}