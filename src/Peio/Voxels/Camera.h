#pragma once

#include "VoxelsHeader.h"
#include "..\Clock.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT Camera {

		Float3 position = {};
		Float2 rotation = {};
		float fov = 0.5f;
		float aspectRatio = 1.0f;

	};

	struct PEIO_VXL_EXPORT SmoothCamera {

		Camera camera = {};
		Clock<float> clock = {};

		float acceleration = 1.0f;
		float retardation = 1.0f;

		void MoveRelative(Float2 rotation);
		void MoveAbsolute(Float2 rotation);
		void Update();

	protected:

		static Float3 RotateX(Peio::Float3 p, float angle);
		static Float3 RotateY(Peio::Float3 p, float angle);

		Float3 velocity = {};
		Float3 dVelocity = {};

	};

}