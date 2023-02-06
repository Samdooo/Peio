#include "..\App.h"
#include "..\..\Windows\KeyboardListener.h"

namespace Input {

	using namespace Peio;

	void State::Init()
	{
		Procedure<Win::KeydownEvent*> keybdProc = [this](Win::KeydownEvent* e) {
			if (e->key == VK_ESCAPE && e->prev == false) {
				this->SetState(!this->active);
			}
		};
		Win::Input::listeners.Add(keybdProc);
		Procedure<Win::WinEvent*> focusProc = [this](Win::WinEvent* e) {
			if (e->msg.message == WM_KILLFOCUS) {
				this->SetState(false);
			}
		};
		Win::Input::listeners.Add(focusProc);
		SetState(false);
	}

	void State::SetState(bool active)
	{
		this->active = active;
		if (active) {
			RECT rect;
			GetWindowRect(App::app.winGraphics.window.GetHWND(), &rect);
			ClipCursor(&rect);
			while (ShowCursor(FALSE) >= 0) {}
		}
		else {
			ClipCursor(nullptr);
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			while (ShowCursor(TRUE) < 0) {}
		}
	}

}