#pragma once

#include "Resource.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ShaderResourceView : public Resource {

		void Init(D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false);
		void Create(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) override;

	protected:

		using Resource::Init;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	};

	struct PEIO_GFX_EXPORT BufferSRV : public ShaderResourceView {

		void Init(UINT64 size, UINT numElements, D3D12_RESOURCE_STATES resourceState, bool copyFootprints = false);

	protected:

		using ShaderResourceView::Init;

	};

	struct PEIO_GFX_EXPORT Texture2DSRV : public ShaderResourceView {

		void Init(Uint2 size, DXGI_FORMAT format, D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_BORDER);

		_NODISCARD D3D12_STATIC_SAMPLER_DESC GetSampler() const noexcept;

	protected:

		using ShaderResourceView::Init;

		D3D12_STATIC_SAMPLER_DESC sampler = {};

	};
	using Texture = Texture2DSRV;

}