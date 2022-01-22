#pragma once

#include "Input.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT KeydownEvent {
		uchar key;
		ushort repeat;
		bool prev;
	};
	struct PEIO_WIN_EXPORT KeyupEvent : public KeydownEvent {};

	struct PEIO_WIN_EXPORT KeyboardListener : public virtual Listener {

		void Handle(Input::Message& msg) override;

	};

}