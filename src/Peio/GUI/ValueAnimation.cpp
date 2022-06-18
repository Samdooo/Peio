#define PEIO_GUI_EXPORTING
#include "ValueAnimation.h"

namespace Peio::GUI {

	double J_Function::Calc(double x) const noexcept
	{
		return 1.0 - std::pow(1.0 - x, multiplier);
	}

	double J_Function::CalcInv(double y) const noexcept
	{
		return 1.0 - std::pow(1.0 - y, 1.0 / multiplier);
	}

	double S_Function::Calc(double x) const noexcept
	{
		return 1.0 / (1.0 + std::pow(x / (1.0 - x), -multiplier));
	}

	double S_Function::CalcInv(double y) const noexcept
	{
		return Calc(y);
	}

}