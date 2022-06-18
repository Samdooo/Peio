#define PEIO_GUI_EXPORTING
#include "PolyAnimation.h"

namespace Peio::GUI {

	void PolyAnimation::Reset(bool reversed, double offset)
	{
		Animation::Reset(reversed, offset);
		for (size_t i = 0; i < animations.size(); i++)
			animations[i]->Reset(offset);
	}

	double PolyAnimation::Update()
	{
		double progress = Animation::Update();
		if (!running)
			return progress;
		for (size_t i = 0; i < animations.size(); i++)
			animations[i]->Update();
		return progress;
	}

	void PolyAnimation::Cancel()
	{
		Animation::Cancel();
		for (size_t i = 0; i < animations.size(); i++)
			animations[i]->Cancel();
	}

	void PolyAnimation::End()
	{
		Animation::End();
		for (size_t i = 0; i < animations.size(); i++)
			animations[i]->End();
	}

}