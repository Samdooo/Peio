#pragma once

#include "Animation.h"
#include "Loadable.h"
#include "..\Files.h"
#include "LoadMap.h"

namespace Peio::GUI {

	template <>
	struct PEIO_GUI_EXPORT Loadable<Animation> {

		static LoadMap<Animation> animationMap;

		static void Load(Animation* obj, std::ifstream& ifile) {
			obj->duration = Files::ReadObject<double>(ifile);
			obj->offset = Files::ReadObject<double>(ifile);
		}

		static void Save(const Animation* obj, std::ofstream& ofile) {
			Files::WriteObject(ofile, obj->duration);
			Files::WriteObject(ofile, obj->offset);
		}

	};
	//template struct PEIO_GUI_EXPORT Loadable<Animation>;

}