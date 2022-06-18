#define PEIO_GUI_EXPORTING
#include "LoadTexture.h"

namespace Peio::GUI {

	std::unordered_map<std::string, Texture*> Loadable<Texture>::textures = {};



}