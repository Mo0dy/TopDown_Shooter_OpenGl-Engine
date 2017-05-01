#include "Camera.h"

Camera::Camera():pos(0.0f, 0.0f, 1.0f)
{
}


Camera::~Camera()
{
}

void Camera::updatePos() {

}

glm::vec3 Camera::getPos() {
	return pos;
}
