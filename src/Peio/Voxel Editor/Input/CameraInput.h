#pragma once

#include "CameraKeybinds.h"

namespace Input {

	struct CameraInput : protected Peio::EventHandler<Peio::Win::RawMouseMoveEvent> {

		void Init();
		void Update();

	protected:

		bool Handle(Peio::Win::RawMouseMoveEvent* event) override;

	};

}