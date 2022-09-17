#pragma once

#include "..\..\..\GUI\Text.h"
#include "..\..\..\GUI\Button.h"

namespace Graphics::GUI {

	struct Place {

		void Init();
		void Render();

		struct Radius : protected Peio::GUI::Button {

			void Init();
			void Update();
			void Render();

			Peio::GUI::Text text = {};

		protected:

			void OnMouseEnter(Peio::Win::MouseMoveEvent*) override;
			void OnMouseLeave(Peio::Win::MouseMoveEvent*) override;
			void OnMouseWheel(Peio::Win::MouseWheelEvent* event) override;

		} radius = {};

		struct Shape : protected Peio::GUI::Button {

			void Init();
			void Update();
			void Render();

			Peio::GUI::Text text = {};

		protected:

			void OnMouseEnter(Peio::Win::MouseMoveEvent*) override;
			void OnMouseLeave(Peio::Win::MouseMoveEvent*) override;
			void OnMouseWheel(Peio::Win::MouseWheelEvent* event) override;

		} shape = {};

	};

}