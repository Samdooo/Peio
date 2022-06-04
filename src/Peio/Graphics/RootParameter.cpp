#define PEIO_GFX_EXPORTING
#include "RootParameter.h"

D3D12_ROOT_PARAMETER Peio::Gfx::RootParameter::CreateConstantBufferView(UINT registerIndex, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	parameter.Descriptor.RegisterSpace = 0;
	parameter.Descriptor.ShaderRegister = registerIndex;
	parameter.ShaderVisibility = visibility;

	return parameter;
}

D3D12_ROOT_PARAMETER Peio::Gfx::RootParameter::CreateShaderResourceView(UINT registerIndex, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	parameter.Descriptor.RegisterSpace = 0;
	parameter.Descriptor.ShaderRegister = registerIndex;
	parameter.ShaderVisibility = visibility;

	return parameter;
}

D3D12_ROOT_PARAMETER Peio::Gfx::RootParameter::CreateUnorderedAccessView(UINT registerIndex, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	parameter.Descriptor.RegisterSpace = 0;
	parameter.Descriptor.ShaderRegister = registerIndex;
	parameter.ShaderVisibility = visibility;

	return parameter;
}

D3D12_ROOT_PARAMETER Peio::Gfx::RootParameter::CreateDescriptorTable(D3D12_ROOT_DESCRIPTOR_TABLE table, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameter.DescriptorTable = table;
	parameter.ShaderVisibility = visibility;

	return parameter;
}
