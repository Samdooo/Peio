#pragma once

#include <functional>
#include "..\..\Windows\RawKeyboardListener.h"
#include "..\..\Windows\RawMouseListener.h"

namespace Input {

	struct Key {
		enum class Type {
			KEYBOARD, MOUSE
		} type = {};
		int key = 0;

		_NODISCARD bool operator==(const Key& that) const;

		struct Hash {
			_NODISCARD size_t operator()(const Key& key) const;
		};
	};

	struct Keybind {

		Key key = {};

		std::function<void()> OnPress = {};
		std::function<void()> OnRelease = {};

		bool pressed = false;

	};

}