#pragma once

#include "..\..\..\GUI\Text.h"
#include "..\..\..\GUI\Button.h"
#include "..\..\..\Windows\TextListener.h"

namespace Graphics::GUI {

	struct Material {

		void Init();
		void Render();

		void UpdateMaterial();

		struct Value : protected Peio::GUI::Button {

			void Init();
			void Update();
			void UpdateMaterial();
			void Render();

			Peio::GUI::Text text = {};
			float* fVal = nullptr;
			int val = 0;
			int maxVal = 255;
			Peio::Clock<double> clock = {};


		protected:

			void OnMouseEnter(Peio::Win::MouseMoveEvent*) override;
			void OnMouseLeave(Peio::Win::MouseMoveEvent*) override;
			void OnMouseWheel(Peio::Win::MouseWheelEvent* event) override;

		};

		Peio::GUI::Text reflectionText = {};
		Peio::GUI::Text emissionText = {};
		Peio::Array<Value, 3> reflection = {};
		Peio::Array<Value, 3> emission = {};

	};

}