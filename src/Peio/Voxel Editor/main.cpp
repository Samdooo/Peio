#include "App.h"

Peio::Double3 RotateX(Peio::Double3 p, double angle) {
    double c = cos(angle);
    double s = sin(angle);
    return Peio::Double3(p.x(), c * p.y() + s * p.z(), -s * p.y() + c * p.z());
}

Peio::Double3 RotateY(Peio::Double3 p, double angle) {
    double c = cos(angle);
    double s = sin(angle);
    return Peio::Double3(c * p.x() - s * p.z(), p.y(), s * p.x() + c * p.z());
}

int main() {

	App::app.Init();
	while (App::app.Update()) {}

	return 0;
}