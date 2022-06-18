#define PEIO_GUI_EXPORTING
#include "Animatable.h"

namespace Peio::GUI {

	void Animatable::AddAnimation(std::string name, Animation* animation)
	{
		animations.insert({ name, animation });
	}

	void Animatable::Update()
	{
		for (auto& a : animations) {
			a.second->Update();
		}
	}

	Animation* Animatable::GetAnimation(std::string name) const
	{
		if (!animations.count(name)) {
			throw PEIO_EXCEPTION("No animation with the specified name was found.");
		}
		return animations.at(name);
	}

}