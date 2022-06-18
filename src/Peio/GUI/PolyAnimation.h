#pragma once

#include "Animation.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT PolyAnimation : public virtual Animation {

		void Reset(bool reversed = false, double offset = 0.0) override;
		double Update() override;
		void Cancel() override;
		void End() override;

		std::vector<Animation*> animations = {};

	};

}