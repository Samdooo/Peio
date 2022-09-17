#pragma once

#include "..\..\..\GUI\Text.h"
#include "..\..\..\GUI\Button.h"

namespace Graphics::GUI {

	struct DenoiseRadius : protected Peio::GUI::Button {

		void Init();
		void Update();
		void Render();

		Peio::GUI::Text text = {};
		UINT maxRadius = 24;

	protected:

		void OnMouseEnter(Peio::Win::MouseMoveEvent*) override;
		void OnMouseLeave(Peio::Win::MouseMoveEvent*) override;
		void OnMouseWheel(Peio::Win::MouseWheelEvent* event) override;

	};

}