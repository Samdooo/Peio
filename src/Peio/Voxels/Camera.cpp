#define PEIO_VXL_EXPORTING
#include "Camera.h"

namespace Peio::Vxl {

	void SmoothCamera::MoveRelative(Float2 rotation)
	{
		MoveAbsolute(camera.rotation + rotation);
	}

	void SmoothCamera::MoveAbsolute(Float2 rotation)
	{
		Float3 ray = { 0.0f, 0.0f, acceleration };
		ray = RotateX(ray, rotation.y());
		ray = RotateY(ray, rotation.x());
		ray *= acceleration;
		dVelocity += ray;
	}

	void SmoothCamera::Update()
	{
		float delta = clock.Restart().Seconds();
		velocity -= velocity * std::min(1.0f, retardation * delta);
		velocity += dVelocity * delta;
		camera.position += velocity * delta;
		dVelocity = {};
	}

	Float3 SmoothCamera::RotateX(Peio::Float3 p, float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return Peio::Float3(p.x(), c * p.y() + s * p.z(), -s * p.y() + c * p.z());
	}

	Float3 SmoothCamera::RotateY(Peio::Float3 p, float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return Peio::Float3(c * p.x() - s * p.z(), p.y(), s * p.x() + c * p.z());
	}

}