module Peio.Essentials.Clock;

using namespace Peio;

Peio::Essentials::Duration::Duration(clk::duration dur)
	: dur(dur)
{}

ll Peio::Essentials::Duration::GetCount()
{
	return dur.count();
}

double Peio::Essentials::Duration::GetLength()
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();
}

Peio::Essentials::Clock::Clock()
{
	start = clk::now();
}

Peio::Essentials::Duration Peio::Essentials::Clock::GetElapsed()
{
	return Duration(clk::now() - start);
}

Peio::Essentials::Duration Peio::Essentials::Clock::Restart()
{
	clk::time_point newStart = clk::now();
	Duration dur = Duration(newStart - start);
	start = newStart;
	return dur;
}
