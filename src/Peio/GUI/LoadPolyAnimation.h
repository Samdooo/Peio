#pragma once

#include "PolyAnimation.h"
#include "LoadAnimation.h"

namespace Peio::GUI {

	template <>
	struct PEIO_GUI_EXPORT Loadable<PolyAnimation> {
		static void Load(PolyAnimation* obj, std::ifstream& ifile) {
			Loadable<Animation>::Load(obj, ifile);
			size_t numAnimations = Files::ReadObject<size_t>(ifile);
			obj->animations.resize(numAnimations);
			for (size_t i = 0; i < numAnimations; i++) {
				obj->animations[i] = Loadable<Animation>::animationMap.Load(ifile);
			}
		}
		static void Save(const PolyAnimation* obj, std::ofstream& ofile) {
			Loadable<Animation>::Save(obj, ofile);
			Files::WriteObject(ofile, obj->animations.size());
			for (size_t i = 0; i < obj->animations.size(); i++) {
				Loadable<Animation>::animationMap.Save(obj->animations[i], ofile);
			}
		}
	};

}