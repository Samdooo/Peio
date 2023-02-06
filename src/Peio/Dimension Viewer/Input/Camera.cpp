#include "..\App.h"

#include "..\..\Windows\RawMouseListener.h"
#include "..\..\Windows\KeyboardListener.h"

#include <iostream>

namespace Input {

	using namespace Peio;

	void Camera::Init() 
	{
		targetVelocity.resize(App::app.scene.numDims, 0.0f);
		velocity.resize(App::app.scene.numDims, 0.0f);

		Procedure<Win::RawMouseMoveEvent*> moveProc = [this](Win::RawMouseMoveEvent* e) {
			if (!App::app.input.state.active)
				return;
			App::app.scene.camera.rotation[rotationDimension] -= (this->mouseSensitivity) * (float)e->movement[1];
			App::app.scene.camera.rotation[rotationDimension + 1] += (this->mouseSensitivity) * (float)e->movement[0];
			//App::app.scene.camera.rotation += { (-this->mouseSensitivity) * (float)e->movement[1], (this->mouseSensitivity) * (float)e->movement[0] };
		};
		Win::Input::listeners.Add(moveProc);

		Procedure<Win::RawMouseWheelEvent*> wheelProc = [this](Win::RawMouseWheelEvent* e) {
			if (!App::app.input.state.active)
				return;
			if (e->delta > 0) {
				rotationDimension++;
				if (rotationDimension == App::app.scene.numDims - 2)
					rotationDimension = 0;
			}
			else if (e->delta < 0) {
				rotationDimension--;
				if (rotationDimension == -1)
					rotationDimension = App::app.scene.numDims - 3;
			}
			std::cout << "Rotation dimensions: " << (rotationDimension + 1) << " " << (rotationDimension + 2) << std::endl;
		};
		Win::Input::listeners.Add(wheelProc);

		Procedure<Win::KeydownEvent*> keyProc = [this](Win::KeydownEvent* e) {
			if (!App::app.input.state.active)
				return;
			if (e->prev)
				return;
			if (e->key == VK_UP) {
				moveDimension++;
				if (moveDimension == App::app.scene.numDims - 2)
					moveDimension = 0;
			}
			else if (e->key == VK_DOWN) {
				moveDimension--;
				if (moveDimension == -1)
					moveDimension = App::app.scene.numDims - 3;
			}
			else return;
			std::cout << "Movement dimensions: " << (moveDimension + 1) << " " << (moveDimension + 2) << " " << (moveDimension + 3) << std::endl;
		};
		Win::Input::listeners.Add(keyProc);

		clock.Restart();
	}

	void Camera::Update()
	{
		targetVelocity *= 0.0f;
		if (App::app.input.state.active) {
			if (GetKeyState('W') & 0x8000) {
				targetVelocity[moveDimension] += speed;
			}
			if (GetKeyState('A') & 0x8000) {
				targetVelocity[moveDimension + 2] -= speed;
			}
			if (GetKeyState('S') & 0x8000) {
				targetVelocity[moveDimension] -= speed;
			}
			if (GetKeyState('D') & 0x8000) {
				targetVelocity[moveDimension + 2] += speed;
			}
			if (GetKeyState(VK_SPACE) & 0x8000) {
				targetVelocity[moveDimension + 1] += speed;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				targetVelocity[moveDimension + 1] -= speed;
			}
			targetVelocity.Rotate(App::app.scene.camera.rotation);
		}

		float delta = clock.Restart().Seconds();
		MathVector<float> diff = targetVelocity - velocity;
		velocity += diff * std::min(delta * acceleration, 1.0f);
		App::app.scene.camera.position += velocity * delta;
	}

}