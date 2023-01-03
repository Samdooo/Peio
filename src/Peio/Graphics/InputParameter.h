#pragma once

#include "Global.h"
#include "Descriptor.h"
#include "DescriptorHeap.h"
#include "..\Group.h"

namespace Peio::Gfx {

	struct PEIO_GFX_EXPORT InputParameter {

		_NODISCARD virtual D3D12_ROOT_PARAMETER CreateParameter() = 0;
		virtual void Set(ID3D12GraphicsCommandList* cmdList) const = 0;

		UINT parameterIndex = 0;
		UINT shaderRegister = 0;
		D3D12_SHADER_VISIBILITY visibility = {};

	protected:

		D3D12_ROOT_PARAMETER param = {};

	};

	struct PEIO_GFX_EXPORT SingleParameter : public InputParameter {
		_NODISCARD D3D12_ROOT_PARAMETER CreateParameter() override;
		void Set(const Resource* single, ID3D12GraphicsCommandList* cmdList) const;
	};

	struct PEIO_GFX_EXPORT SingleResourceParameter : public SingleResource, public SingleParameter {
		void Set(ID3D12GraphicsCommandList* cmdList) const override;
	};

	struct PEIO_GFX_EXPORT ArrayParameter : public InputParameter {
		_NODISCARD D3D12_ROOT_PARAMETER CreateParameter() override;
		void Set(const Resource* array, ID3D12GraphicsCommandList* cmdList) const;
	};

	struct PEIO_GFX_EXPORT ArrayBufferParameter : public ArrayBufferResource, public ArrayParameter {
		void Set(ID3D12GraphicsCommandList* cmdList) const override;
	};

	struct PEIO_GFX_EXPORT RWArrayParameter : public InputParameter {
		_NODISCARD D3D12_ROOT_PARAMETER CreateParameter() override;
		void Set(const Resource* array, ID3D12GraphicsCommandList* cmdList) const;
	};

	struct PEIO_GFX_EXPORT RWArrayBufferParameter : public RWArrayBufferResource, public RWArrayParameter {
		void Set(ID3D12GraphicsCommandList* cmdList) const override;
	};

	struct PEIO_GFX_EXPORT TableParameter : public DescriptorHeap, public InputParameter {

		void Init(UINT numDescriptors);
		void Put(Group<Descriptor, InputParameter> descriptor, UINT index);
		

		void Set(ID3D12GraphicsCommandList* cmdList) const override;

	protected:

		using DescriptorHeap::Put;

		std::vector<Group<Descriptor, InputParameter>> descriptors = {};

	};

}