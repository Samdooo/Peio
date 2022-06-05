#pragma once

#include "Texture.h"
#include "..\Graphics\PipelineStateHeader.h"
#include "..\Graphics\ResourceArray.h"
#include "..\Graphics\ConstantBufferView.h"
#include "..\Graphics\VertexBuffer.h"
#include "..\Graphics\Graphics.h"
#include "Uploadable.h"
#include "Drawable.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Rect {
		Uint2 offset = {};
		Uint2 position = {};
		Uint2 size = {};
		BOOL useColor = false, useTexture = false, useAlpha = false;
	};

	struct PEIO_GUI_EXPORT RectVertex {
		Float2 position = {};
		Float4 color = {};
		Float2 texCoord = {};
		Float2 alphaCoord = {};
	};

	struct PEIO_GUI_EXPORT RectWindow {
		Uint2 size = {};
	};

	struct PEIO_GUI_EXPORT Rectangle : public Uploadable, public Drawable {

		void Init(Gfx::Graphics* graphics, Uint2 position, Uint2 size, Uint2 offset = {});

		void SetColor(Float4 color);
		void SetTexture(Texture* texture);
		void SetAlphaTexture(Texture* alphaTexture);

		void Upload() override;
		void Draw() override;

		_NODISCARD Rect& GetRect() const noexcept;

	protected:

		Gfx::Graphics* graphics = nullptr;

		Gfx::PipelineState pso = {};

		Gfx::VertexBuffer<RectVertex> vertices = {};
		Gfx::SubresourceBuffer<Rect> rect = {};
		Gfx::SubresourceBuffer<RectWindow> rectWindow = {};

		Gfx::ConstantBufferView rectBuffer = {};
		Gfx::ConstantBufferView rectWindowBuffer = {};
		Gfx::ResourceArray cbvArray = {};

		Texture* texture = nullptr;
		Texture* alphaTexture = nullptr;

		ID3D12DescriptorHeap* descriptorHeaps[2] = {};

		D3D12_VIEWPORT viewPort = {};
		RECT scissorRect = {};

	};

}