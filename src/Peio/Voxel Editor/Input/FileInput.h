#pragma once

#include "..\..\Windows\Input.h"

namespace Input {

	struct FileInput : protected Peio::EventHandler<Peio::Win::WinMessageEvent, Peio::Win::RawKeyUpEvent> {

		void Init();
		void SaveWorld();

	protected:

		bool Handle(Peio::Win::WinMessageEvent* event) override;
		bool Handle(Peio::Win::RawKeyUpEvent* event) override;

	};

}