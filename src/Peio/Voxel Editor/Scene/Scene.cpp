#include "..\App.h"
#include "Scene.h"

namespace Scene {

	void Scene::Init()
	{
		info.Init();
		world.Init();
		camera.Init();
		rayField.Init();
	}

	void Scene::Update()
	{
		world.Update();
		camera.Update();
		info.Update();
	}

}