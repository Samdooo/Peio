#pragma once

#include "Animation.h"

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT AnimationFunction {
		
		virtual ~AnimationFunction() {}

		_NODISCARD virtual double Calc(double x) const noexcept = 0;
		_NODISCARD virtual double CalcInv(double y) const noexcept = 0;

	};
	
	/// <summary>
	/// Uses the function (1 - (1 - x)^m)
	/// </summary>
	struct PEIO_GUI_EXPORT J_Function : public virtual AnimationFunction {

		_NODISCARD double Calc(double x) const noexcept override;
		_NODISCARD double CalcInv(double y) const noexcept override;

		double multiplier = 1.0;

	};

	/// <summary>
	/// Uses the function (1 / (1 + (x / (1 - x))^(-m)))
	/// </summary>
	struct PEIO_GUI_EXPORT S_Function : public virtual AnimationFunction {

		_NODISCARD double Calc(double x) const noexcept override;
		_NODISCARD double CalcInv(double y) const noexcept override;

		double multiplier = 1.0;

	};

	template <typename T>
	struct ValueAnimation : public virtual Animation {

		//void Reset(double offset = 0.0) override {
		//	Animation::Reset(function->CalcInv(offset));
		//}

		double Update() override {
			double progress = Animation::Update();
			if (!running)
				return progress;
			current.resize(numValues);
			double val = function->Calc(progress);
			for (size_t i = 0; i < numValues; i++) {
				current[i] = (T)((1.0 - val) * (double)from[i] + val * (double)to[i]);
			}
			UpdateValues(&current[0]);
			return progress;
		}

		void End() override {
			if (running)
				UpdateValues(&to[0]);
			Animation::End();
		}

		AnimationFunction* function = nullptr;

		std::vector<T> from = {}, to = {};
		size_t numValues = 0;
		std::function<void(const T*)> UpdateValues = nullptr;

	protected:

		std::vector<T> current = {};

	};

}