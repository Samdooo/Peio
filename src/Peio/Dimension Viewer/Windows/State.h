#pragma once

namespace Windows {

	struct State {

		bool active = false;
		int toggleKey = VK_ESCAPE;
		int refreshKey = VK_F1;
		int logKey = VK_F3;

		void Init();
		void SetState(bool active);
		void LogState();

	};

}