#include "Camera.h"

Camera::Camera():pos(0.0f, 0.0f)
{
}


Camera::~Camera()
{
}

void Camera::updatePos(float width, float height) {
	size = glm::vec2(1, 1);
}

