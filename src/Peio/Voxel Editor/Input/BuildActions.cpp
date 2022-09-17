#include "..\App.h"

namespace Input {

	void BuildActions::Init()
	{
		place = {
			"Place",
			[this]() {
				Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
				Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);

				if (ray.material == Scene::World::MaterialMap::null)
					return;

				// TODO: Fix overflows

				Peio::Int3 center = (Peio::Int3)ray.voxel;

				if (ray.normal[ray.side] < 0.0)
					center[ray.side]--;
				else
					center[ray.side]++;

				UINT materialIndex = App::app.scene.world.GetMaterialIndex(place.material);

				for (int x = center.x() - place.radius; x <= center.x() + place.radius; x++) {
					for (int y = center.y() - place.radius; y <= center.y() + place.radius; y++) {
						for (int z = center.z() - place.radius; z <= center.z() + place.radius; z++) {
							App::app.scene.world.SetVoxel((Peio::Uint3)Peio::Int3{ x, y, z }, materialIndex);
						}
					}
				}
			}
		};

		destroy = {
			"Destroy",
			[this]() {
				Peio::Vxl::Camera camera = App::app.scene.camera.smoothCam.camera;
				Scene::World::Ray ray = App::app.scene.world.Trace(camera.position, camera.rotation);

				if (ray.material == Scene::World::MaterialMap::null)
					return;

				// TODO: Fix overflows

				Peio::Int3 center = (Peio::Int3)ray.voxel;

				if (ray.normal[ray.side] < 0.0)
					center[ray.side]--;
				else
					center[ray.side]++;

				for (int x = center.x() - destroy.radius; x <= center.x() + destroy.radius; x++) {
					for (int y = center.y() - destroy.radius; y <= center.y() + destroy.radius; y++) {
						for (int z = center.z() - destroy.radius; z <= center.z() + destroy.radius; z++) {
							App::app.scene.world.SetVoxel((Peio::Uint3)Peio::Int3{ x, y, z }, Scene::World::MaterialMap::null);
						}
					}
				}
			}
		};
		App::app.input.actions.actions.push_back(&place);
		App::app.input.actions.actions.push_back(&destroy);
	}

}