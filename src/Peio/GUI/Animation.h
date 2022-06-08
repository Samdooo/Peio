#pragma once

#include "GUIHeader.h"
#include "..\Array.h"
#include "..\Clock.h"
#include <functional>

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Animation {

		virtual void Reset(double offset = 0.0);
		virtual double Update();
		virtual void Cancel();
		virtual void End();

		virtual _NODISCARD double GetProgress() const noexcept;

		double duration = 0.0;
		double offset = 0.0;

	protected:

		bool running = false;
		double curOffset = 0.0;
		Clock<double> clock = {};

	};

}