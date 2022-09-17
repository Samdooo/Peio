#pragma once

#include "VoxelsHeader.h"
#include "..\Clock.h"

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT Camera {

		Float3 position = {};
		Float2 rotation = {};
		float fov = 0.5f;
		float aspectRatio = 1.0f;

		_NODISCARD bool operator==(const Camera& that) const;

	};

	struct PEIO_VXL_EXPORT SmoothCamera {

		Camera camera = {};
		Clock<float> clock = {};

		Float3 velocity = {};
		float targetVelocity = 1.0f;
		float minVelocity = 0.5f;
		Float3 targetDirection = {};
		float acceleration = 1.0f;
		float sensitivity = 0.001f;

		void Rotate(Float2 rotation);
		void Update();

	protected:

		static Float3 RotateX(Peio::Float3 p, float angle);
		static Float3 RotateY(Peio::Float3 p, float angle);


	};

}