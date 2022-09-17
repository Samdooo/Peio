#pragma once

#include "StateKeybinds.h"

namespace Input {

	struct StateInput : protected Peio::Win::Listener {

		void Init();

		void SetState(State state);
		void TogglePause();

	protected:

		bool Handle(Peio::Win::WinMessageEvent* event) override;

	};

}