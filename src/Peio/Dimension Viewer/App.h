#pragma once

#include "Config/Config.h"
#include "Scene/Scene.h"
#include "Renderer/RayRenderer.h"
#include "Renderer/Denoiser.h"
#include "Windows/WinGraphics.h"
#include "Video/VideoGraphics.h"
#include "Windows/Input.h"

struct App {

	static App app;

	bool init = false;

	Config::Config config = {};
	Scene::Scene scene = {};
	Renderer::RayRenderer rayRenderer = {};
	Renderer::Denoiser denoiser = {};
	Windows::WinGraphics winGraphics = {};
	Video::VideoGraphics videoGraphics = {};
	bool isVideo = false;

	void Init();
	bool Update();
	void Cleanup();

};