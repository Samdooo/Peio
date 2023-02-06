#pragma once

#include "..\MathVector.h"
#include "..\..\Array.h"
#include "..\..\Graphics\InputParameter.h"

namespace Scene {

	using namespace Peio;

	struct Camera {

		MathVector<float> position = {};
		MathVector<float> rotation = {};
		float fov = 1.0f;
		float aspectRatio = 1.0f;

		Gfx::ArrayBufferParameter buffer = {};
		
		void Init();
		void Update(ID3D12GraphicsCommandList* cmdList);

	};

}