#pragma once

#include "Window.h"
#include "RayTracer.h"
#include "Denoiser.h"
#include "GUI/GUI.h"
#include "..\..\Graphics\WinGraphics.h"

namespace Graphics {

	struct Graphics {

		void Init();
		void Update();

		_NODISCARD ID3D12GraphicsCommandList* GetCommandList() const;

		Peio::Gfx::WinGraphics graphics = {};

		Window window = {};
		RayTracer rayTracer = {};
		Denoiser denoiser = {};
		GUI::GUI gui = {};

	};

}