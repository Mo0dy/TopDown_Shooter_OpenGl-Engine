#include "Camera.h"
#include "Util.h"
#include <iostream>

Camera::Camera():pos(-1.0f, -1.0f)
{
}

Camera::~Camera()
{
}

void Camera::updatePos(GLfloat width, GLfloat height, std::vector<Player*> players) {
	
	GLfloat WHratio = width / height;

	glm::vec2 maxPPos = players[0]->pos;
	glm::vec2 minPPos = maxPPos;

	for (int i = 1; i < players.size(); i++) {
		if (players[i]->pos.x < minPPos.x) {
			minPPos.x = players[i]->pos.x;
		}
		else if (players[i]->pos.x > maxPPos.x) {
			maxPPos.x = players[i]->pos.x;
		}
		if (players[i]->pos.y < minPPos.y) {
			minPPos.y = players[i]->pos.y;
		}
		else if (players[i]->pos.y > maxPPos.y) {
			maxPPos.y = players[i]->pos.y;
		}
	}
	
	minPPos = minPPos - glm::vec2(OFFSET, OFFSET);
	maxPPos = maxPPos + glm::vec2(OFFSET, OFFSET);

	size = maxPPos - minPPos;
	glm::vec2 camMid = minPPos + 0.5f * size;

	if (abs(size.y) * WHratio > abs(size.x)) { // width is too small
		size.x = size.y * WHratio;
	}
	else { // heigth is too small
		size.y = size.x / WHratio;
	}

	if (abs(size).y < MIN_SIZE_HEIGHT) {
		size *= MIN_SIZE_HEIGHT / abs(size).y;
	}

	pos = camMid - 0.5f * size;
}

