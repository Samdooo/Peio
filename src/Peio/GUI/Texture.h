#pragma once

#include "GUIHeader.h"

#include "..\Graphics\ShaderResourceView.h"
#include "..\Graphics\ResourceArray.h"
#include "..\Graphics\SubresourceBuffer.h"
#include "..\Media\Images.h"
#include "..\Graphics\Graphics.h"
#include "Uploadable.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Texture : public Uploadable {

		void Init(Gfx::Graphics* graphics, Med::Frame frame, DXGI_FORMAT format);
		void Upload() override;

		_NODISCARD Med::Frame GetFrame() const noexcept;
		_NODISCARD const Gfx::ResourceArray& GetResourceArray() const noexcept;
		_NODISCARD const Gfx::Texture2DSRV& GetSRV() const noexcept;

	protected:

		Gfx::Graphics* graphics = nullptr;

		Med::Frame frame = {};

		Gfx::ResourceArray resourceArray = {};
		Gfx::Texture2DSRV texture = {};
		Gfx::SubresourceBuffer<byte> textureData = {};

	};

}