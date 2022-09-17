#define PEIO_VXL_EXPORTING
#include "Camera.h"

namespace Peio::Vxl {

	bool Camera::operator==(const Camera& that) const 
	{
		return aspectRatio == that.aspectRatio &&
			fov == that.fov &&
			position == that.position &&
			rotation == that.rotation;
	}

	void SmoothCamera::Rotate(Float2 rotation)
	{
		camera.rotation += rotation * sensitivity;
		camera.rotation.x() = fmodf(camera.rotation.x(), 6.2831853071f);
		camera.rotation.y() = fmodf(camera.rotation.y(), 6.2831853071f);
	}

	void SmoothCamera::Update()
	{
		float delta = clock.Restart().Seconds();
		Float3 diff = (targetDirection * targetVelocity) - velocity;
		velocity += diff * std::min(delta * acceleration, 1.0f);

		Float3 ray = velocity;
		ray = RotateX(ray, camera.rotation.y());
		ray = RotateY(ray, camera.rotation.x());
		camera.position += ray * delta;
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