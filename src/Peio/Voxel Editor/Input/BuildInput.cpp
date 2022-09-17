#include "..\App.h"
#include <random>

namespace Input {

	void BuildInput::Init()
	{
		place.Init();
		destroy.Init();
		pick.Init();
		fill.Init();
	}

	void BuildInput::Update()
	{
		fill.Update();
	}

	void BuildInput::PlaceInput::Init()
	{
		App::app.input.keybinds.build.place.OnPress = [this]() { Place(); };
	}

	void BuildInput::PlaceInput::Place()
	{
		if (App::app.state != State::FREECAM)
			return;

		Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
		Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);
		if (ray.material == Scene::World::MaterialMap::null)
			return;

		Peio::Int3 center = ray.voxel;

		if (ray.normal[ray.side] < 0.0)
			center[ray.side]--;
		else
			center[ray.side]++;

		for (int x = center.x() - radius; x <= center.x() + radius; x++) {
			for (int y = center.y() - radius; y <= center.y() + radius; y++) {
				for (int z = center.z() - radius; z <= center.z() + radius; z++) {
					if (shape == Shape::SPHERE) {
						Peio::Int3 off = Peio::Int3(x, y, z) - center;
						double dist = sqrt(off.x() * off.x() + off.y() * off.y() + off.z() * off.z());
						if (dist >= (double)radius)
							continue;
					}
					App::app.scene.world.SetVoxel({ (UINT)x, (UINT)y, (UINT)z }, material);
				}
			}
		}
	}

	void BuildInput::DestroyInput::Init()
	{
		App::app.input.keybinds.build.destroy.OnPress = [this]() { Destroy(); };
	}

	void BuildInput::DestroyInput::Destroy()
	{
		if (App::app.state != State::FREECAM)
			return;

		Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
		Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);
		if (ray.material == Scene::World::MaterialMap::null)
			return;

		Peio::Int3 center = ray.voxel;

		for (int x = center.x() - radius; x <= center.x() + radius; x++) {
			for (int y = center.y() - radius; y <= center.y() + radius; y++) {
				for (int z = center.z() - radius; z <= center.z() + radius; z++) {
					if (shape == Shape::SPHERE) {
						Peio::Int3 off = Peio::Int3(x, y, z) - center;
						double dist = sqrt(off.x() * off.x() + off.y() * off.y() + off.z() * off.z());
						if (dist >= (double)radius)
							continue;
					}
					App::app.scene.world.SetVoxel({ (UINT)x, (UINT)y, (UINT)z }, Scene::World::MaterialMap::null);
				}
			}
		}
	}

	void BuildInput::PickInput::Init()
	{
		App::app.input.keybinds.build.pick.OnPress = [this]() { Pick(); };
	}

	void BuildInput::PickInput::Pick()
	{
		if (App::app.state != State::FREECAM)
			return;

		Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
		Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);
		if (ray.material == Scene::World::MaterialMap::null)
			return;

		App::app.input.build.place.material = App::app.scene.world.GetMaterial(ray.material);
		App::app.graphics.gui.material.UpdateMaterial();
	}

	void BuildInput::FillInput::Init()
	{
		App::app.input.keybinds.build.fill.OnPress = [this]() { Fill(); };
	}

	void BuildInput::FillInput::Fill()
	{
		if (App::app.state != State::FREECAM)
			return;

		Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
		Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);
		if (ray.material == Scene::World::MaterialMap::null)
			return;

		Peio::Int3 center = ray.voxel;
		
		double yOrigin = center.y();

		std::uniform_real_distribution<double> random(-randomNess, randomNess);
		std::default_random_engine randomGen = {};

		for (int y = std::max(0, center.y() - scanHeight); y <= center.y() + (int)scanHeight; y++) {
			for (int x = center.x() - scanWidth; x <= center.x() + scanWidth; x++) {
				for (int z = center.z() - scanWidth; z <= center.z() + scanWidth; z++) {
					UINT material = App::app.scene.world.GetVoxel({ (UINT)x, (UINT)y, (UINT)z });
					if (material != ~0)
						continue;
					Peio::Int3 off = Peio::Int3(x, y, z) - center;
					double fact = 1.0 - (sqrt(off.x() * off.x() + off.y() * off.y() + off.z() * off.z()) / targetWidth);
					fact += (double)(center.y() - y) / targetHeight;
					fact += random(randomGen);
					if (fact > 0.5) {
						App::app.scene.world.SetVoxel({ (UINT)x, (UINT)y, (UINT)z }, ray.material);
					}
				}
			}
		}
	}

	void BuildInput::FillInput::Update()
	{
		if (App::app.input.keybinds.build.fill.pressed) {
			if (clock.Elapsed().Seconds() >= delay) {
				Fill();
				clock.Restart();
			}
		}
	}

}