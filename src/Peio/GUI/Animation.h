#pragma once

#include "GUIHeader.h"
#include "..\Array.h"
#include "..\Clock.h"
#include <functional>

namespace Peio::GUI {

	template <typename T, size_t length>
	struct Animation {

		void Init(Array<T, length> from, Array<T, length> to, double duration, std::function<void(Array<T, length>)> update) {
			this->from = from;
			this->to = to;
			this->duration = duration;
			this->update = update;
		}

		void Reset() {
			clock.Restart();
		}

		void Update() {
			Array<T, length> current = {};
			double progress = min(clock.Elapsed() / duration, 1.0);
			progress = Calc(progress);
			for (size_t i = 0; i < length; i++) {
				current[i] = (T)((1.0 - progress) * (double)from[i] + progress * (double)to[i]);
			}
			update(current);
		}

	protected:

		virtual double Calc(double) = 0;

		Clock<double> clock = {};

		Array<T, length> from = {}, to = {};
		double duration = 0.0;
		std::function<void(Array<T, length>)> update = nullptr;

	};

	template <typename T, size_t length>
	struct J_Animation : public Animation<T, length> {

		double multiplier = 1.0;

	protected:

		double Calc(double x) {
			return 1.0 - std::pow(1.0 - x, multiplier);
		}

	};

	template <typename T, size_t length>
	struct S_Animation : public Animation<T, length> {

		double multiplier = 1.0;

	protected:

		double Calc(double x) {
			return 1.0 / (1.0 + std::pow(x / (1.0 - x), -multiplier));
		}

	};

}