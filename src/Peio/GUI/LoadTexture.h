#pragma once

#include "Texture.h"
#include "Loadable.h"
#include <unordered_map>
#include "..\Files.h"

namespace Peio::GUI {

	template <>
	struct PEIO_GUI_EXPORT Loadable<Texture> {

		static void Load(Texture* obj, std::ifstream& ifile) {
			std::string filePath = Files::ReadString(ifile);
			if (textures.count(filePath)) {
				
			}
			else {

			}
		}

		static void Save(Texture* obj, std::ofstream& ofile) {
			Files::WriteString(ofile, obj->GetFrame().filePath);
		}

	protected:

		static std::unordered_map<std::string, Texture*> textures;

	};

}