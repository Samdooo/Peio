#pragma once

#include "..\Scene\Camera.h"
#include <vector>

namespace Video {

	struct Path {

		struct Point {
			MathVector<float> position = {};
			MathVector<float> rotation = {};
		};

		std::vector<Point> points = {};

		void Init();

		bool Update();

	};

}