export module Peio.Essentials.Clock;

import Peio.Essentials.Types;
export import <chrono>;

export namespace Peio {

	namespace Essentials {

		class Duration {

			using clk = std::chrono::steady_clock;
			clk::duration dur;

		public:

			Duration(clk::duration dur);

			ll GetCount();
			double GetLength();

		};

		class Clock {
			
			using clk = std::chrono::steady_clock;
			clk::time_point start;

		public:

			Clock();

			Duration GetElapsed();
			Duration Restart();

		};

	}
	using namespace Essentials;

}