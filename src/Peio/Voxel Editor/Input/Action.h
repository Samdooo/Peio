#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace Input {

	struct Action {

		std::string name = {};
		std::function<void()> function = {};

		void operator()();

	};

}

