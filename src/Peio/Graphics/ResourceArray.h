#pragma once

#include "ShaderResource.h"
#include "DescriptorHeap.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT ResourceArray {

		void Init(D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void AddResource(ShaderResource* resource);
		void CreateResources();

		_NODISCARD ID3D12DescriptorHeap** GetDescriptorHeaps() const noexcept;
		_NODISCARD DescriptorHeap& GetDescriptorHeap() noexcept;
		_NODISCARD const DescriptorHeap& GetDescriptorHeap() const noexcept;
		_NODISCARD ShaderResource** GetResources() noexcept;
		_NODISCARD UINT GetNumResources() const noexcept;
		_NODISCARD D3D12_SHADER_VISIBILITY GetVisibility() const noexcept;

		void Release();
		~ResourceArray();

	protected:

		ID3D12DescriptorHeap** descriptorHeaps = new ID3D12DescriptorHeap*[1];
		DescriptorHeap descriptorHeap = {};
		std::vector<ShaderResource*> resources = {};
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL;

	};

}