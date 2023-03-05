#include "..\App.h"
#include "..\..\Windows\KeyboardListener.h"

#include <iostream>

namespace Windows {

	using namespace Peio;

	void State::Init()
	{
		if (!App::app.init) {
			Procedure<Win::KeydownEvent*> keybdProc = [this](Win::KeydownEvent* e) {
				if (e->prev)
					return;
				if (e->key == toggleKey) {
					this->SetState(!this->active);
				}
				if (e->key == refreshKey) {
					try {
						App::app.Init();
						std::cout << "Refreshed" << std::endl;
					}
					catch (Peio::Exception e) {
						std::cout << "Exception when refreshing: \"" << e.msg << "\" in " << e.file << " line " << e.line << std::endl;
					}
				}
				if (e->key == logKey) {
					LogState();
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
		else {
			SetState(active);
		}
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

	void State::LogState()
	{
		std::cout << "Camera position: " << App::app.scene.camera.position.ToString() << std::endl;
		std::cout << "Camera rotation: " << App::app.scene.camera.rotation.ToString() << std::endl;
	}

}