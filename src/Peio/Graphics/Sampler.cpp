#define PEIO_GFX_EXPORTING
#include "Sampler.h"

namespace Peio::Gfx {

	D3D12_STATIC_SAMPLER_DESC Sampler::Create(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode)
	{
		D3D12_STATIC_SAMPLER_DESC sampler = {};
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
		sampler.ShaderRegister = shaderRegister;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = visibility;
		return sampler;
	}

}