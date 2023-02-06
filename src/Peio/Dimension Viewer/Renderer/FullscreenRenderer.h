#pragma once

#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\VertexBuffer.h"

namespace Renderer {

	using namespace Peio;

	struct FullscreenRenderer : public Gfx::Renderer {

		void Create();
		void Update(ID3D12GraphicsCommandList* cmdList);
		void Render(ID3D12GraphicsCommandList* cmdList);

	protected:

		Gfx::VertexBuffer vertexBuffer = {};
		D3D12_VIEWPORT viewPort = {};
		RECT scissorRect = {};

	};

}