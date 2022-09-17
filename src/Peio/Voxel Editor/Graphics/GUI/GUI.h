#pragma once

#include "Fonts.h"
#include "Framerate.h"
#include "RayCount.h"
#include "DenoiseRadius.h"
#include "Speed.h"
#include "Material.h"
#include "Place.h"
#include "Destroy.h"
#include "Fill.h"

namespace Graphics::GUI {

	struct GUI {

		void Init();
		void Update();
		void Render();

		Fonts fonts = {};
		Framerate framerate = {};
		RayCount rayCount = {};
		DenoiseRadius denoiseRadius = {};
		Speed speed = {};
		Material material = {};
		Place place = {};
		Destroy destroy = {};
		Fill fill = {};

		Peio::Win::MouseListener mouseListener = {};
		Peio::Win::TextListener textListener = {};

	};

}