#pragma once

#include "Input.h"

namespace Peio::Win {

	struct PEIO_WIN_EXPORT TextEvent {
		char character;
		ushort repeat;
		bool prev;
	};

	struct PEIO_WIN_EXPORT TextListener : public virtual Listener {

		void Handle(Input::Message& msg) override;

	};

}