#pragma once

#include "Exception.h"
#include <chrono>

namespace Peio {

	template <typename T_rep>
	struct Clock {

		using sysclock = std::chrono::system_clock;

		struct Duration : public sysclock::duration {
			using _duration = sysclock::duration;

			Duration(const _duration& d) : _duration(d) {}

			_NODISCARD T_rep Seconds() const { return std::chrono::duration_cast<std::chrono::duration<T_rep>>(*this).count(); }
		};

		_NODISCARD Duration Elapsed() noexcept {
			return Duration(sysclock::now() - start);
		}

		_NODISCARD Duration Restart() noexcept {
			Duration result((sysclock::duration)(sysclock::now() - start));
			start += result;
			return result;
		}

	protected:

		sysclock::time_point start = sysclock::now();

	};

}