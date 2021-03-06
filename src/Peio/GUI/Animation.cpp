#define PEIO_GUI_EXPORTING
#include "Animation.h"

namespace Peio::GUI {

	void Animation::Reset(bool reversed, double offset)
	{
		this->reversed = reversed;
		this->curOffset = offset;
		if (reversed)
			this->curOffset = 1.0 - this->curOffset;
		clock.Restart();
		running = true;
	}

	double Animation::Update()
	{
		if (!running)
			return 0.0;
		double progress = GetProgress();
		if (progress == 1.0) {
			End();
		}
		return progress;
	}

	void Animation::Cancel()
	{
		running = false;
	}

	void Animation::End()
	{
		running = false;
	}

	double Animation::GetProgress() const noexcept
	{
		double progress = std::max(std::min((clock.Elapsed().Seconds() - offset + curOffset) / duration, 1.0), 0.0);
		return (reversed ? (1.0 - progress) : progress);
	}

}