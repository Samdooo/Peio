#pragma once

#include "Animation.h"
#include <unordered_map>
#include <string>

namespace Peio::GUI {

	struct PEIO_GUI_EXPORT Animatable {

		void AddAnimation(std::string name, Animation* animation);

		void Update();

		_NODISCARD Animation* GetAnimation(std::string name) const;

	protected:

		std::unordered_map<std::string, Animation*> animations = {};

	};

}