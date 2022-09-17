#include "..\App.h"
#include "CameraInput.h"

namespace Input {

	void CameraInput::Init()
	{
		Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());
	}

	void CameraInput::Update()
	{
		Peio::Float3 dir = {};
		if (App::app.state == State::FREECAM) {
			if (App::app.input.keybinds.camera.moveForward.pressed)
				dir.z() += 1.0f;
			if (App::app.input.keybinds.camera.moveBackward.pressed)
				dir.z() -= 1.0f;
			if (App::app.input.keybinds.camera.moveLeft.pressed)
				dir.x() -= 1.0f;
			if (App::app.input.keybinds.camera.moveRight.pressed)
				dir.x() += 1.0f;
			if (App::app.input.keybinds.camera.moveUp.pressed)
				dir.y() += 1.0f;
			if (App::app.input.keybinds.camera.moveDown.pressed)
				dir.y() -= 1.0f;
		}
		App::app.scene.camera.smoothCam.targetDirection = dir;
	}

	bool CameraInput::Handle(Peio::Win::RawMouseMoveEvent* event)
	{
		if (App::app.state == State::FREECAM) {
			App::app.scene.camera.smoothCam.Rotate(Peio::Float2() - ((Peio::Float2)event->movement));
			RECT rect = App::app.graphics.window.window.GetRect();
			Peio::Int2 center = { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };
			SetCursorPos(center.x(), center.y());
		}
		return false;
	}

}