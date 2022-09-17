#pragma once

#include "State.h"
#include "Scene/Scene.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"

struct App {

	static App app;

	void Init();
	bool Update();

	State state = State::PAUSED;
	Graphics::Graphics graphics = {};
	Scene::Scene scene = {};
	Input::Input input = {};

};