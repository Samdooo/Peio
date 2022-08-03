#define PEIO_GFX_EXPORTING
#include "ShaderResourceView.h"

void Peio::Gfx::ShaderResourceView::Init(D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints)
{
	this->srvDesc = srvDesc;
	Resource::Init(resourceDesc, resourceState, copyFootprints);
}

void Peio::Gfx::ShaderResourceView::Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
{
	device->CreateShaderResourceView(GetBuffer(), &srvDesc, cpuHandle);
}

void Peio::Gfx::ShaderResourceView::SetAsRootParameter(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) const
{
	cmdList->SetGraphicsRootShaderResourceView(rootParameterIndex, this->GetGPUAddress());
}

void Peio::Gfx::BufferSRV::Init(UINT64 size, UINT numElements, D3D12_RESOURCE_STATES resourceState, bool copyFootprints)
{
	srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = (UINT)numElements;
	srvDesc.Buffer.StructureByteStride = (UINT)(size / numElements);

	Resource::Init(CD3DX12_RESOURCE_DESC::Buffer(size), resourceState, copyFootprints);
}

void Peio::Gfx::Texture2DSRV::Init(Uint2 size, DXGI_FORMAT format, D3D12_RESOURCE_STATES resourceState, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode)
{
	srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = textureAddressMode;
	sampler.AddressV = textureAddressMode;
	sampler.AddressW = textureAddressMode;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0; // Should be set by the RootSignature
	sampler.RegisterSpace = 0;

	Resource::Init(CD3DX12_RESOURCE_DESC::Tex2D(format, (UINT64)size.x(), size.y()), resourceState, true);
}

D3D12_STATIC_SAMPLER_DESC Peio::Gfx::Texture2DSRV::GetSampler() const noexcept
{
	return sampler;
}
