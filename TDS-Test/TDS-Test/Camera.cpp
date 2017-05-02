#include "Camera.h"

Camera::Camera():pos(-1.0f, -1.0f)
{
}


Camera::~Camera()
{
}

void Camera::updatePos(float width, float height) {
	float scale = 2 / height;
	size = glm::vec2(scale * width, scale * height);
}

