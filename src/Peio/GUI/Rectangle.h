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

	struct PEIO_GUI_EXPORT RectVertex {
		Float2 position = {};
		Float4 color = {};
		Float2 texCoord = {};
		Float2 alphaCoord = {};
	};

	struct PEIO_GUI_EXPORT RectWindow {
		Uint2 size = {};
	};

	struct PEIO_GUI_EXPORT Rect {
		Uint2 position = {};
		Uint2 size = {};
		BOOL useColor = false, useTexture = false, useAlpha = false, useOffset = false;
	};

	struct PEIO_GUI_EXPORT Rectangle : public Uploadable, public Drawable {

		static Gfx::PipelineState pipelineState;
		static void Init();

		void Init(Gfx::Graphics* graphics, Uint2 position, Uint2 size);

		void SetColor(Float4 color);
		void SetTexture(const Texture* texture);
		void SetAlphaTexture(const Texture* alphaTexture);

		void Upload() override;
		void Draw() override;

		_NODISCARD Rect& GetRect() const noexcept;

	protected:

		Gfx::Graphics* graphics = nullptr;

		Gfx::VertexBuffer<RectVertex> vertices = {};
		Gfx::SubresourceBuffer<Rect> rect = {};
		Gfx::SubresourceBuffer<RectWindow> rectWindow = {};

		Gfx::ConstantBufferView rectBuffer = {};
		Gfx::ConstantBufferView rectWindowBuffer = {};

		const Texture* texture = nullptr;
		const Texture* alphaTexture = nullptr;

		D3D12_VIEWPORT viewPort = {};
		RECT scissorRect = {};

	};

	struct PEIO_GUI_EXPORT RectOffset {
		Uint2 offset = {};
		Uint2 size = {};
	};

	struct PEIO_GUI_EXPORT RectangleOffset : public Uploadable, public Drawable {

		void Init(Gfx::Graphics* graphics, Uint2 offset, Uint2 size);

		void Upload() override;
		void Draw() override;

		_NODISCARD RectOffset& GetOffset() const noexcept;

	protected:

		Gfx::Graphics* graphics = nullptr;

		Gfx::SubresourceBuffer<RectOffset> offset = {};
		Gfx::ConstantBufferView offsetBuffer = {};

	};

}