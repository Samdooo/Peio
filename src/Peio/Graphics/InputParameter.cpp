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

	D3D12_ROOT_PARAMETER TableParameter::CreateParameter()
	{
		param = {};
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ShaderVisibility = visibility;
		
		ranges.clear();
		for (UINT i = 0; i < descriptors.size(); i++) {
			D3D12_ROOT_PARAMETER p = descriptors[i].Get<InputParameter>()->CreateParameter();
			D3D12_DESCRIPTOR_RANGE_TYPE type;
			switch (p.ParameterType) {
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
				type = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
				type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
				type = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
				break;
			}
			if (ranges.size() && ranges.back().RangeType == type && ranges.back().BaseShaderRegister + ranges.back().NumDescriptors == p.Descriptor.ShaderRegister) {
				ranges.back().NumDescriptors++;
			}
			else {
				D3D12_DESCRIPTOR_RANGE range = {};
				range.BaseShaderRegister = p.Descriptor.ShaderRegister;
				range.NumDescriptors = 1;
				range.OffsetInDescriptorsFromTableStart = ranges.size() ? (ranges.back().OffsetInDescriptorsFromTableStart + ranges.back().NumDescriptors) : 0;
				range.RangeType = type;
				range.RegisterSpace = 0;
				ranges.push_back(range);
			}
		}
		param.DescriptorTable.NumDescriptorRanges = ranges.size();
		param.DescriptorTable.pDescriptorRanges = &ranges[0];

		return param;
	}

	void TableParameter::Set(ID3D12GraphicsCommandList* cmdList) const
	{
		ID3D12DescriptorHeap* heaps[1] = { heap.Get() };
		cmdList->SetDescriptorHeaps(1, heaps);
		cmdList->SetGraphicsRootDescriptorTable(parameterIndex, DescriptorHeap::GetGPUHandle());
	}

}