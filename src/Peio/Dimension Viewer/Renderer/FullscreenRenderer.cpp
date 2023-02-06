#include "..\App.h"

namespace Renderer {

	void FullscreenRenderer::Create()
	{
		Renderer::vertexLayout.elements = {
			Gfx::VertexElement{"POSITION", DXGI_FORMAT_R32G32_FLOAT}
		};
		Renderer::Create();

		vertexBuffer.Init(sizeof(Float2), 6);
		std::vector<Float2> vertices(6);
		for (UINT i = 0; i < 6; i++) {
			vertices[i].x() = (i >= 1 && i <= 3) ? 1.0f : -1.0f;
			vertices[i].y() = (i >= 2 && i <= 4) ? -1.0f : 1.0f;
		}
		memcpy(vertexBuffer.buffer.GetData(), &vertices[0], sizeof(Float2) * vertices.size());

		viewPort = { 0.0f, 0.0f, (float)App::app.scene.info.screenSize.x(), (float)App::app.scene.info.screenSize.y(), 0.0f, 1.0f };
		scissorRect = { 0, 0, (LONG)App::app.scene.info.screenSize.x(), (LONG)App::app.scene.info.screenSize.y() };
	}

	void FullscreenRenderer::Update(ID3D12GraphicsCommandList* cmdList)
	{
		vertexBuffer.Upload(cmdList);
	}

	void FullscreenRenderer::Render(ID3D12GraphicsCommandList* cmdList)
	{
		Renderer::Render(cmdList, &vertexBuffer, viewPort, scissorRect);
	}

}


