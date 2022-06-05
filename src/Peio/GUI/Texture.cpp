#define PEIO_GUI_EXPORTING
#include "Texture.h"

namespace Peio::GUI {

	void Texture::Init(Gfx::Graphics* graphics, Med::Frame frame, DXGI_FORMAT format)
	{
		this->graphics = graphics;
		texture.Init(frame.GetSize(), format, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		this->frame = frame;
		textureData.SetBuffer(frame.GetData(), frame.GetBytesPP() * frame.GetSize().x(), frame.GetSize().y());

		resourceArray.AddResource(&texture);
		resourceArray.CreateResources();
	}

	void Texture::Upload()
	{
		texture.Upload(textureData.GetResourceData(), graphics->GetCommandList());
	}

	Med::Frame Texture::GetFrame() const noexcept
	{
		return frame;
	}

	const Gfx::ResourceArray& Texture::GetResourceArray() const noexcept
	{
		return resourceArray;
	}

	const Gfx::Texture2DSRV& Texture::GetSRV() const noexcept
	{
		return texture;
	}

}