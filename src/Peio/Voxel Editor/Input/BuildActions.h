#pragma once

#include "Action.h"
#include "..\..\Voxels\Material.h"

namespace Input {

	struct BuildActions {

		enum class Shape {
			CUBE, SPHERE
		};

		struct PlaceAction : public Action {

			UINT radius = 1;
			Shape shape = Shape::CUBE;
			Peio::Vxl::Material material = {};

		} place;

		struct DestroyAction : public Action {

			UINT radius = 1;
			Shape shape = Shape::CUBE;

		} destroy;

		void Init();

	};

}