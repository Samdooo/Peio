#define PEIO_GUI_EXPORTING
#include "LoadValueAnimation.h"

namespace Peio::GUI {

	//template struct Loadable<AnimationFunction>;
	//template struct Loadable<J_Function>;
	//template struct Loadable<S_Function>;
	//template struct Loadable<ValueAnimation<float>>;
	//template struct Loadable<ValueAnimation<double>>;
	//template struct Loadable<ValueAnimation<int>>;

	LoadMap<AnimationFunction> Loadable<AnimationFunction>::functionMap = LoadMap<AnimationFunction>()
		.Insert<J_Function, 0>()
		.Insert<S_Function, 1>();

}