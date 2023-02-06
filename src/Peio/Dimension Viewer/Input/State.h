#pragma once

namespace Input {

	struct State {

		bool active = false;

		void Init();
		void SetState(bool active);

	};

}