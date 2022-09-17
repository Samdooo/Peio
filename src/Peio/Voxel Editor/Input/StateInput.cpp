#include "..\App.h"

namespace Input {

	void StateInput::Init()
	{
		App::app.input.keybinds.state.pause.OnPress = [this]() { TogglePause(); };
		Peio::Win::Input::AddListener(this);
	}

	void StateInput::SetState(State state)
	{
		App::app.state = state;
		if (state == State::FREECAM) {
			while (ShowCursor(FALSE) >= 0) {}
		}
		else {
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			while (ShowCursor(TRUE) < 0) {}
		}
	}

	void StateInput::TogglePause()
	{
		if (App::app.state == State::UNFOCUSED)
			return;
		if (App::app.state == State::FREECAM)
			SetState(State::PAUSED);
		else if (App::app.state == State::PAUSED)
			SetState(State::FREECAM);
	}
	
	bool StateInput::Handle(Peio::Win::WinMessageEvent* event)
	{
		if (event->msg.message == WM_SETFOCUS) {
			SetState(State::PAUSED);
		}
		else if (event->msg.message == WM_KILLFOCUS) {
			SetState(State::UNFOCUSED);
		}
		return false;
	}

}