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

	struct PEIO_GUI_EXPORT RectangleInfo {
		BOOL useColor = false, useTexture = false, useAlpha = false;
	};

	struct PEIO_GUI_EXPORT Rectangle : public Uploadable, public Drawable {

		static Gfx::PipelineState pipelineState;
		static void Init();

		void Init(Gfx::Graphics* graphics, Float2 position, Float2 size);

		void SetColor(Float4 color);
		void SetTexture(const Texture* texture);
		void SetAlphaTexture(const Texture* alphaTexture);

		void Upload() override;
		void Draw() override;

		_NODISCARD Float2& GetPosition() noexcept;
		_NODISCARD Float2& GetSize() noexcept;
		_NODISCARD RectangleInfo& GetInfo() const noexcept;

	protected:

		Gfx::Graphics* graphics = nullptr;

		Gfx::VertexBuffer<RectVertex> vertices = {};
		Gfx::SubresourceBuffer<RectangleInfo> info = {};

		Gfx::ConstantBufferView infoBuffer = {};

		D3D12_VIEWPORT viewPort = {};

		const Texture* texture = nullptr;
		const Texture* alphaTexture = nullptr;
		
		RECT scissorRect = {};

	};

}