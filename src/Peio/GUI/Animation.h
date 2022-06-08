#pragma once

#include "GUIHeader.h"
#include "..\Array.h"
#include "..\Clock.h"
#include <functional>

namespace Peio::GUI {

	template <typename T, size_t length>
	struct Animation {

		void Reset() {
			clock.Restart();
		}

		_NODISCARD double GetProgress() const noexcept {
			return std::min(clock.Elapsed().Seconds() / duration, 1.0);
		}

		void Update() {
			Array<T, length> current = {};
			double progress = Calc(GetProgress());
			for (size_t i = 0; i < length; i++) {
				current[i] = (T)((1.0 - progress) * (double)from[i] + progress * (double)to[i]);
			}
			update(current);
		}

		Array<T, length> from = {}, to = {};
		double duration = 0.0;
		std::function<void(Array<T, length>)> update = nullptr;

	protected:

		virtual double Calc(double) = 0;

		Clock<double> clock = {};

	};

	template <typename T, size_t length>
	struct J_Animation : public virtual Animation<T, length> {

		double multiplier = 1.0;

	protected:

		double Calc(double x) {
			return 1.0 - std::pow(1.0 - x, multiplier);
		}

	};

	template <typename T, size_t length>
	struct S_Animation : public virtual Animation<T, length> {

		double multiplier = 1.0;

	protected:

		double Calc(double x) {
			return 1.0 / (1.0 + std::pow(x / (1.0 - x), -multiplier));
		}

	};

}