#include "Camera.h"

Camera::Camera():pos(-1.0f, -1.0f)
{
}


Camera::~Camera()
{
}

void Camera::updatePos(float width, float height, glm::vec2 goalPos) {
	
	float metersPerHeight = 10;

	float scale = metersPerHeight / height; 
	size = glm::vec2(scale * width, scale * height);

	pos = goalPos - glm::vec2(5, 5);
}

