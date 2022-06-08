#pragma once

#include "Animation.h"

namespace Peio::GUI {

	template <typename T, size_t length>
	struct ValueAnimation : public virtual Animation {

		void Reset(double offset = 0.0) override {
			Animation::Reset(CalcInv(offset));
		}

		double Update() override {
			double progress = Animation::Update();
			if (!running)
				return progress;
			Array<T, length> current = {};
			double val = Calc(progress);
			for (size_t i = 0; i < length; i++) {
				current[i] = (T)((1.0 - val) * (double)from[i] + val * (double)to[i]);
			}
			UpdateValue(current);
			return progress;
		}

		void End() override {
			if (running)
				UpdateValue(to);
			Animation::End();
		}

		virtual double Calc(double) = 0;
		virtual double CalcInv(double) = 0;

		Array<T, length> from = {}, to = {};
		std::function<void(Array<T, length>)> UpdateValue = nullptr;
	};

	template <typename T, size_t length>
	struct J_Animation : public virtual ValueAnimation<T, length> {

		double multiplier = 1.0;

		double Calc(double x) override {
			return 1.0 - std::pow(1.0 - x, multiplier);
		}

		double CalcInv(double y) override {
			return 1.0 - std::pow(1.0 - y, 1.0 / multiplier);
		}

	};

	template <typename T, size_t length>
	struct S_Animation : public virtual ValueAnimation<T, length> {

		double multiplier = 1.0;

		double Calc(double x) override {
			return 1.0 / (1.0 + std::pow(x / (1.0 - x), -multiplier));
		}

		double CalcInv(double y) override {
			return Calc(y);
		}

	};

}