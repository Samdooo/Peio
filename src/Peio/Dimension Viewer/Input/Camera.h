#pragma once

#include "..\..\Clock.h"

namespace Input {

	using namespace Peio;

	struct Camera {

		Clock<float> clock = {};
		MathVector<float> targetVelocity = {};
		MathVector<float> velocity = {};
		
		float speed = 10.0f;
		float acceleration = 10.0f;

		float mouseSensitivity = 0.001f;
		UINT rotationDimension = 0;
		UINT moveDimension = 0;

		void Init();
		void Update();



	};

}