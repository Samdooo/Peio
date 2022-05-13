#define PEIO_GFX_EXPORTING
#include "RootSignature.h"

//ID3D12RootSignature* Peio::Gfx::RootSignature::Create(const std::vector<D3D12_ROOT_PARAMETER>& params, const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers, D3D12_ROOT_SIGNATURE_FLAGS flags)
//{
//	HRESULT ret;
//
//	ID3D12RootSignature* rootSignature = nullptr;
//
//	D3D12_ROOT_PARAMETER* pParams = nullptr;
//	if (params.size()) {
//		pParams = new D3D12_ROOT_PARAMETER[params.size()];
//		memcpy(pParams, &params[0], sizeof(D3D12_ROOT_PARAMETER) * params.size());
//	}
//
//	D3D12_STATIC_SAMPLER_DESC* pSamplers = nullptr;
//	if (samplers.size()) {
//		pSamplers = new D3D12_STATIC_SAMPLER_DESC[samplers.size()];
//		memcpy(pSamplers, &samplers[0], sizeof(D3D12_STATIC_SAMPLER_DESC) * samplers.size());
//	}
//
//	CD3DX12_ROOT_SIGNATURE_DESC desc;
//	desc.Init((UINT)params.size(), pParams, (UINT)samplers.size(), pSamplers, flags);
//
//	ID3DBlob* signature;
//	ret = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
//	if (ret != 0) {
//		throw PEIO_GFX_EXCEPTION("Failed to serialize root signature.", ret);
//	}
//
//	ret = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature), reinterpret_cast<void**>(&rootSignature));
//	if (ret != 0) {
//		throw PEIO_GFX_EXCEPTION("Failed to create root signature.", ret);
//	}
//
//	return rootSignature;
//}

void Peio::Gfx::RootSignature::Init(D3D12_ROOT_SIGNATURE_FLAGS flags)
{
	std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};
	UINT srvIndex = 0;
	for (UINT i = 0; i < srvs.size(); i++) {
		for (UINT j = 0; j < srvs[i].GetNumResources(); j++) {
			rootParameters.push_back(RootParameter::CreateShaderResourceView(srvIndex++, srvs[i].GetVisibility()));
		}
	}
	UINT uavIndex = 1;
	for (UINT i = 0; i < uavs.size(); i++) {
		for (UINT j = 0; j < uavs[i].GetNumResources(); j++) {
			rootParameters.push_back(RootParameter::CreateUnorderedAccessView(uavIndex++, uavs[i].GetVisibility()));
		}
	}

	HRESULT ret;
	
	CD3DX12_ROOT_SIGNATURE_DESC desc;
	desc.Init((UINT)rootParameters.size(), &rootParameters[0], (UINT)samplers.size(), &samplers[0], flags);
	
	ID3DBlob* signature;
	ret = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to serialize root signature.", ret);
	}
	
	ret = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature), &rootSignature);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create root signature.", ret);
	}
}

void Peio::Gfx::RootSignature::SetRootSignature(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	UINT index = 0;
	for (UINT i = 0; i < srvs.size(); i++) {
		for (UINT j = 0; j < srvs[i].GetNumResources(); j++) {
			cmdList->SetGraphicsRootShaderResourceView(index++, srvs[i].GetResources()[j].GetGPUAddress());
		}
	}
	for (UINT i = 0; i < uavs.size(); i++) {
		for (UINT j = 0; j < uavs[i].GetNumResources(); j++) {
			cmdList->SetGraphicsRootUnorderedAccessView(index++, uavs[i].GetResources()[j].GetGPUAddress());
		}
	}
}

ID3D12RootSignature* Peio::Gfx::RootSignature::GetRootSignature() const noexcept
{
	return rootSignature.Get();
}
