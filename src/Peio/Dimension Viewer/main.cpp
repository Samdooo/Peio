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
			if (App::app.scene.objects.hyperRectangles.rects.size()) {
				//App::app.scene.objects.hyperRectangles.rects[0].rotation[0] += 0.019f;
				//App::app.scene.objects.hyperRectangles.rects[0].rotation[1] += 0.012f;
				//if (App::app.scene.numDims > 3) {
				//	App::app.scene.objects.hyperRectangles.rects[0].rotation[2] += 0.01f;
				//}
			}
			if (App::app.isVideo) {
				if (clock.Elapsed().Seconds() >= 1.0) {
					clock.Restart();
					std::cout << App::app.videoGraphics.encoder.GetFrameIndex() << "/" << App::app.videoGraphics.path.points.size() << " frames rendered" << std::endl;
				}
			}
			else {
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