#include "App.h"
#include "MathVector.h"

#include <iostream>
#include <filesystem>

#define PI 3.1415926535898f
#define PHI 1.6180339887499f

int main(int argc, char* argv[]) {

	try {

		Peio::Clock<double> clock;
		double framerate = 40.0;

		if (argc >= 2) {
			App::app.config.configPath = std::string(argv[1]) + "/";
		}

		App::app.Init();
		while (App::app.Update()) {
			if (App::app.isVideo) {
				int frameIndex = App::app.videoGraphics.encoder.GetFrameIndex();
				if (App::app.scene.objects.hyperRectangles.rects.size()) {
					//App::app.scene.objects.hyperRectangles.rects[0].rotation[0] = 2.0f * PI * (float)frameIndex / 360.0f;
					//App::app.scene.objects.hyperRectangles.rects[0].rotation[1] = 2.0f * PI * (float)frameIndex / 540.0f;
					//if (App::app.scene.numDims > 3) {
					//	App::app.scene.objects.hyperRectangles.rects[0].rotation[2] = 2.0f * PI * (float)frameIndex / 720.0f;
					//}
				}
				if (clock.Elapsed().Seconds() >= 1.0) {
					clock.Restart();
					std::cout << frameIndex << "/" << App::app.videoGraphics.path.points.size() << " frames rendered" << std::endl;
				}
			}
			else {
				if (App::app.scene.objects.hyperRectangles.rects.size()) {
					//App::app.scene.objects.hyperRectangles.rects[0].rotation[0] += 1.0f / 80.0f;
					//App::app.scene.objects.hyperRectangles.rects[0].rotation[1] += 1.0f / 70.0f;
					//if (App::app.scene.numDims > 3) {
					//	App::app.scene.objects.hyperRectangles.rects[0].rotation[2] += 1.0f / 75.0f;
					//}
				}
				while (clock.Elapsed().Seconds() < (1.0 / framerate)) {}
				clock.Restart();
			}
		}
		std::cout << App::app.videoGraphics.path.points.size() << "/" << App::app.videoGraphics.path.points.size() << " frames rendered" << std::endl;
		App::app.Cleanup();
	}
	catch (Peio::Gfx::Exception e) {
		std::cout << "Gfx Exception: \"" << e.msg << "\" in " << e.file << " line " << e.line << std::endl;
		std::cin.get();
	}
	catch (Peio::Exception e) {
		std::cout << "Exception: \"" << e.msg << "\" in " << e.file << " line " << e.line << std::endl;
		std::cin.get();
	}
	catch (...) {
		std::cout << "Unknown exception." << std::endl;
		std::cin.get();
	}

	return 0;
}