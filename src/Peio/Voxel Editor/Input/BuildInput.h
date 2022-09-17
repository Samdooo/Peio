#pragma once

#include "BuildKeybinds.h"
#include "..\..\Voxels\Material.h"

namespace Input {

	struct BuildInput {

		void Init();
		void Update();

		enum class Shape {
			CUBE = 0, SPHERE
		};

		struct PlaceInput {
			void Init();
			void Place();
			int radius = 5;
			Shape shape = Shape::CUBE;
			Peio::Vxl::Material material = { { 0.5f, 0.5f, 0.5f, 1.0f }, {} };
		};
		struct DestroyInput {
			void Init();
			void Destroy();
			int radius = 5;
			Shape shape = Shape::CUBE;
		};
		struct PickInput {
			void Init();
			void Pick();
		};
		struct FillInput {
			void Init();
			void Fill();
			void Update();
			double targetWidth = 25.0;
			double targetHeight = 10.0;
			int scanWidth = 40;
			int scanHeight = 10;
			double randomNess = 0.03;
			double delay = 0.1;
			Peio::Clock<double> clock = {};
		};

		PlaceInput place = {};
		DestroyInput destroy = {};
		PickInput pick = {};
		FillInput fill = {};

	};

}