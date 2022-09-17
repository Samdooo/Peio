#pragma once

#include "..\..\Voxels\Camera.h"
#include "..\..\Graphics\SubresourceBuffer.h"
#include "..\..\Graphics\ShaderResourceView.h"

namespace Scene {

	struct Camera {

		void Init();
		void Upload();
		void Update();
		
		Peio::Vxl::SmoothCamera smoothCam = {};
		Peio::Gfx::SubresourceBuffer<Peio::Vxl::Camera> buffer = {};
		Peio::Gfx::BufferSRV srv = {};
		Peio::Vxl::Camera lastUpload = {};

	};

}