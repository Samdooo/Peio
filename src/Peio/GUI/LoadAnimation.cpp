#define PEIO_GUI_EXPORTING
#include "LoadAnimation.h"
#include "LoadValueAnimation.h"
#include "LoadPolyAnimation.h"

namespace Peio::GUI {

	//template struct Loadable<Animation>;

	LoadMap<Animation> Loadable<Animation>::animationMap = LoadMap<Animation>()
		.Insert<ValueAnimation<float>, 0>()
		.Insert<ValueAnimation<double>, 1>()
		.Insert<ValueAnimation<int>, 2>()
		.Insert<PolyAnimation, 3>();

}