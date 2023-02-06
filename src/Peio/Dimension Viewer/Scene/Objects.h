#pragma once

#include "..\MathVector.h"
#include "..\..\Array.h"
#include "..\..\Graphics\InputParameter.h"

namespace Scene {

	using namespace Peio;

	struct Material {
		Float3 emission = {};
		Float3 reflection = {};
		float specular = 0.0f;
	};

	struct HyperRectangle {
		MathVector<float> low = {};
		MathVector<float> high = {};
		Material material = {};
	};
	struct HyperRectangles {

		std::vector<HyperRectangle> rects = {};
		Gfx::ArrayBufferParameter buffer = {};

		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

	};

	struct HyperSphere {
		MathVector<float> center = {};
		float radius = 0.0f;
		Material material = {};
	};
	struct HyperSpheres {

		std::vector<HyperSphere> spheres = {};
		Gfx::ArrayBufferParameter buffer = {};

		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

	};

	struct Objects {

		HyperRectangles hyperRectangles = {};
		HyperSpheres hyperSpheres = {};

		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

	};

}