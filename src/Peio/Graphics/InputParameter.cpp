#define PEIO_GFX_EXPORTING
#include "InputParameter.h"

namespace Peio::Gfx {

	D3D12_ROOT_PARAMETER SingleParameter::CreateParameter()
	{
		param = {};
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.ShaderVisibility = visibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		param.Descriptor.RegisterSpace = 0;
		return param;
	}

	void SingleParameter::Set(const Resource* single, ID3D12GraphicsCommandList* cmdList) const
	{
		cmdList->SetGraphicsRootConstantBufferView(parameterIndex, single->GetGPUAddress());
	}

	void SingleResourceParameter::Set(ID3D12GraphicsCommandList* cmdList) const
	{
		SingleParameter::Set(this, cmdList);
	}

	D3D12_ROOT_PARAMETER ArrayParameter::CreateParameter()
	{
		param = {};
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		param.ShaderVisibility = visibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		param.Descriptor.RegisterSpace = 0;
		return param;
	}

	void ArrayParameter::Set(const Resource* array, ID3D12GraphicsCommandList* cmdList) const
	{
		cmdList->SetGraphicsRootShaderResourceView(parameterIndex, array->GetGPUAddress());
	}

	D3D12_ROOT_PARAMETER RWArrayParameter::CreateParameter()
	{
		param = {};
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		param.ShaderVisibility = visibility;
		param.Descriptor.ShaderRegister = shaderRegister;
		param.Descriptor.RegisterSpace = 0;
		return param;
	}

	void RWArrayParameter::Set(const Resource* array, ID3D12GraphicsCommandList* cmdList) const
	{
		cmdList->SetGraphicsRootUnorderedAccessView(parameterIndex, array->GetGPUAddress());
	}

	void ArrayBufferParameter::Set(ID3D12GraphicsCommandList* cmdList) const
	{
		ArrayParameter::Set(this, cmdList);
	}

	void RWArrayBufferParameter::Set(ID3D12GraphicsCommandList* cmdList) const
	{
		RWArrayParameter::Set(this, cmdList);
	}

	void TableParameter::Init(UINT numDescriptors)
	{
		DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		descriptors.resize(numDescriptors);
	}

	void TableParameter::Put(Group<Descriptor, InputParameter> descriptor, UINT index)
	{
		DescriptorHeap::Put(descriptor, index);
		descriptors[index] = descriptor;
	}

	void TableParameter::Set(ID3D12GraphicsCommandList* cmdList) const
	{
		ID3D12DescriptorHeap* heaps[1] = { heap.Get() };
		cmdList->SetDescriptorHeaps(1, heaps);
		cmdList->SetGraphicsRootDescriptorTable(parameterIndex, DescriptorHeap::GetGPUHandle());
	}



	

}