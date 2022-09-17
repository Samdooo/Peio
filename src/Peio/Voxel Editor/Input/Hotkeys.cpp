#include "..\App.h"

void Input::Hotkeys::Init()
{
	Peio::Win::RawKeyboardListener::Register(App::app.graphics.window.window.GetHWND());
	Peio::Win::Input::AddListener(&listener);
	Peio::Win::Input::eventHandlers.Insert(this, GetBaseHandler<Peio::Win::WinEvent>());

	hotkeys.insert({
		'W',
		{ 'W', App::app.input.actions.cameraActions.moveForward, App::app.input.actions.cameraActions.moveBackward}
		});
	hotkeys.insert({
		'A',
		{ 'A', App::app.input.actions.cameraActions.moveLeft, App::app.input.actions.cameraActions.moveRight}
		});
	hotkeys.insert({
		'S',
		{ 'S', App::app.input.actions.cameraActions.moveBackward, App::app.input.actions.cameraActions.moveForward}
		});
	hotkeys.insert({
		'D',
		{ 'D', App::app.input.actions.cameraActions.moveRight, App::app.input.actions.cameraActions.moveLeft}
		});
	hotkeys.insert({
		VK_SPACE,
		{ 'S', App::app.input.actions.cameraActions.moveUp, App::app.input.actions.cameraActions.moveDown}
		});
	hotkeys.insert({
		VK_SHIFT,
		{ 'D', App::app.input.actions.cameraActions.moveDown, App::app.input.actions.cameraActions.moveUp}
		});
}

bool Input::Hotkeys::Handle(Peio::Win::RawKeyDownEvent* event)
{
	if (hotkeys.contains(event->key))
		hotkeys.at(event->key).actionPressed();
	return false;
}

bool Input::Hotkeys::Handle(Peio::Win::RawKeyUpEvent* event)
{
	if (hotkeys.contains(event->key))
		hotkeys.at(event->key).actionReleased();
	return false;
}
