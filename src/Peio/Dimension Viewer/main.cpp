#include "App.h"
#include "MathVector.h"

#define PI 3.1415926535898f

int main() {

	/* TESTING CODE */

	App::app.scene.numDims = 3;
	App::app.scene.maxBounces = 2;
	App::app.scene.rays.perBounce = { 16 };
	App::app.scene.info.denoiser.radius = 8;
	App::app.scene.info.denoiser.maxDist = 0.3f;
	App::app.scene.info.denoiser.maxAngle = 0.1f;

	App::app.scene.info.screenSize = { 1280, 720 };
	App::app.scene.camera.position = MathVector<float>(App::app.scene.numDims, 0.0f);
	App::app.scene.camera.rotation = MathVector<float>(App::app.scene.numDims - 1, 0.0f);
	App::app.scene.camera.fov = PI / 2.0f;
	App::app.scene.camera.aspectRatio = (float)App::app.scene.info.screenSize.y() / (float)App::app.scene.info.screenSize.x();
	
	App::app.input.camera.speed = 10.0f;
	App::app.input.camera.acceleration = 10.0f;

	auto RandFloat = []() -> float {
		return 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
	};

	{
		float dist = 4.0f;
		for (UINT i = 0; i < 10; i++) {
			Scene::HyperSphere sphere;
			sphere.center.resize(App::app.scene.numDims);
			for (UINT j = 0; j < App::app.scene.numDims; j++)
				sphere.center[j] = RandFloat() * dist;
			sphere.radius = 1.0f + RandFloat() * 0.5f;
			sphere.material.reflection = { abs(RandFloat()), abs(RandFloat()), abs(RandFloat()) };
			App::app.scene.objects.hyperSpheres.spheres.push_back(sphere);
		}
		float size = 8.0f;
		for (UINT i = 0; i < 10; i++) {
			Scene::HyperRectangle rect;
			rect.low.resize(App::app.scene.numDims);
			rect.high.resize(App::app.scene.numDims);
			for (UINT i = 0; i < App::app.scene.numDims; i++) {
				rect.low[i] = RandFloat() * dist;
				rect.high[i] = rect.low[i] + abs(RandFloat()) * size;
			}
			rect.material.reflection = { abs(RandFloat()), abs(RandFloat()), abs(RandFloat()) };
			App::app.scene.objects.hyperRectangles.rects.push_back(rect);
		}
	}

	Peio::Clock<double> clock;
	double framerate = 40.0;

	App::app.Init();
	while (App::app.Update()) {
		while (clock.Elapsed().Seconds() < (1.0 / framerate)) {}
		clock.Restart();
	}

	return 0;
}