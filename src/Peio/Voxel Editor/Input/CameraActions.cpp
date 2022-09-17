#include "..\App.h"

namespace Input {

	void CameraActions::Init()
	{
		moveForward = {
			"Move camera forward",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.z() += 1.0f;
			}
		};
		moveBackward = {
			"Move camera backward",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.z() -= 1.0f;
			}
		};
		moveLeft = {
			"Move camera left",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.x() -= 1.0f;
			}
		};
		moveRight = {
			"Move camera right",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.x() += 1.0f;
			}
		};
		moveUp = {
			"Move camera up",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.y() += 1.0f;
			}
		};
		moveDown = {
			"Move camera down",
			[]() {
				App::app.scene.camera.smoothCam.targetDirection.y() -= 1.0f;
			}
		};
		App::app.input.actions.actions.push_back(&moveForward);
		App::app.input.actions.actions.push_back(&moveBackward);
		App::app.input.actions.actions.push_back(&moveLeft);
		App::app.input.actions.actions.push_back(&moveRight);
		App::app.input.actions.actions.push_back(&moveUp);
		App::app.input.actions.actions.push_back(&moveDown);
	}

}