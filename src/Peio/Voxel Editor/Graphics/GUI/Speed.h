#pragma once

#include "..\..\..\GUI\Text.h"
#include "..\..\..\GUI\Button.h"

namespace Graphics::GUI {

	struct Speed : protected Peio::GUI::Button {

		void Init();
		void Update();
		void Render();

		Peio::GUI::Text text = {};
		float speedMultiplier = 1.1f;

	protected:

		void OnMouseEnter(Peio::Win::MouseMoveEvent*) override;
		void OnMouseLeave(Peio::Win::MouseMoveEvent*) override;
		void OnMouseWheel(Peio::Win::MouseWheelEvent* event) override;

	};

}