#include "..\App.h"

#include "..\..\Windows\RawMouseListener.h"
#include "..\..\Windows\KeyboardListener.h"

#include <iostream>

namespace Windows {

	using namespace Peio;

	void Camera::Init() 
	{
		if (!App::app.init) {
			Procedure<Win::RawMouseMoveEvent*> moveProc = [this](Win::RawMouseMoveEvent* e) {
				if (!App::app.winGraphics.input.state.active)
					return;
				if (App::app.scene.numDims == 2) {
					App::app.scene.camera.rotation[rotationDimension] += (this->mouseSensitivity) * (float)e->movement[0];
				}
				else {
					App::app.scene.camera.rotation[rotationDimension] -= (this->mouseSensitivity) * (float)e->movement[1];
					App::app.scene.camera.rotation[rotationDimension + 1] += (this->mouseSensitivity) * (float)e->movement[0];
				}
			};
			Win::Input::listeners.Add(moveProc);

			Procedure<Win::RawMouseWheelEvent*> wheelProc = [this](Win::RawMouseWheelEvent* e) {
				if (!App::app.winGraphics.input.state.active)
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
				if (!App::app.winGraphics.input.state.active)
					return;
				if (e->prev)
					return;
				if (e->key == moveDimensionKeys.first) {
					moveDimension++;
					if (moveDimension == App::app.scene.numDims - App::app.winGraphics.input.camera.movementKeys.size() + 1)
						moveDimension = 0;
				}
				else if (e->key == moveDimensionKeys.second) {
					moveDimension--;
					if (moveDimension == -1)
						moveDimension = App::app.scene.numDims - App::app.winGraphics.input.camera.movementKeys.size();
				}
				else if (e->key == VK_LEFT) {
					rotationDimension++;
					if (rotationDimension == App::app.scene.numDims - 2)
						rotationDimension = 0;
				}
				else if (e->key == VK_RIGHT) {
					rotationDimension--;
					if (rotationDimension == -1)
						rotationDimension = App::app.scene.numDims - 3;
				}
				else return;
				std::cout << "Movement dimensions: " << (moveDimension + 1) << " " << (moveDimension + 2) << " " << (moveDimension + 3) << std::endl;
				std::cout << "Rotation dimensions: " << (rotationDimension + 1) << " " << (rotationDimension + 2) << std::endl;
			};
			Win::Input::listeners.Add(keyProc);
		}
		if (targetVelocity.size() != App::app.scene.numDims)
			targetVelocity.resize(App::app.scene.numDims, 0.0f);
		if (velocity.size() != App::app.scene.numDims)
			velocity.resize(App::app.scene.numDims, 0.0f);
		clock.Restart();
	}

	void Camera::Update()
	{
		targetVelocity *= 0.0f;
		if (App::app.winGraphics.input.state.active) {
			for (size_t i = 0; i < std::min(movementKeys.size(), (size_t)App::app.scene.numDims); i++) {
				if (GetKeyState(movementKeys[i].first) & 0x8000)
					targetVelocity[moveDimension + i] += speed;
				if (GetKeyState(movementKeys[i].second) & 0x8000)
					targetVelocity[moveDimension + i] -= speed;
			}
			targetVelocity.Rotate(App::app.scene.camera.rotation);
		}

		float delta = clock.Restart().Seconds();
		MathVector<float> diff = targetVelocity - velocity;
		velocity += diff * std::min(delta * acceleration, 1.0f);
		App::app.scene.camera.position += velocity * delta;
	}

}