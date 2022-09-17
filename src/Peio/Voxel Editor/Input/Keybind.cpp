#include "Keybind.h"

namespace Input {

	bool Key::operator==(const Key& that) const
	{
		return type == that.type && key == that.key;
	}

	size_t Key::Hash::operator()(const Key& key) const
	{
		return std::hash<Type>()(key.type) ^ std::hash<int>()(key.key);
	}

}