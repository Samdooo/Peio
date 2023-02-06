#pragma once

#include "Scene/Scene.h"
#include "Renderer/RayRenderer.h"
#include "Renderer/Denoiser.h"
#include "Graphics/WinGraphics.h"
#include "Input/Input.h"

struct App {

	static App app;

	Scene::Scene scene = {};
	Renderer::RayRenderer rayRenderer = {};
	Renderer::Denoiser denoiser = {};
	Graphics::WinGraphics winGraphics = {};
	Input::Input input = {};

	void Init();
	bool Update();

};