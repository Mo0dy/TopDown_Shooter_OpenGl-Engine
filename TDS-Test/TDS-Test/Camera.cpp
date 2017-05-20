#include "Camera.h"
#include "Util.h"
#include <iostream>

Camera::Camera():pos(-1.0f, -1.0f), minSizeHeight(10)
{
}

Camera::~Camera()
{
}

void Camera::updatePos(GLfloat width, GLfloat height, std::vector<Player*> players) {
	
	GLfloat WHratio = width / height;

	glm::vec2 maxPPos = players[0]->Get2DPos();
	glm::vec2 minPPos = maxPPos;

	// finds the min and maximal x and y values
	for (int i = 1; i < players.size(); i++) {
		if (players[i]->Get2DPos().x < minPPos.x) {
			minPPos.x = players[i]->Get2DPos().x;
		}
		else if (players[i]->Get2DPos().x > maxPPos.x) {
			maxPPos.x = players[i]->Get2DPos().x;
		}
		if (players[i]->Get2DPos().y < minPPos.y) {
			minPPos.y = players[i]->Get2DPos().y;
		}
		else if (players[i]->Get2DPos().y > maxPPos.y) {
			maxPPos.y = players[i]->Get2DPos().y;
		}
	}
	
	// adds offset so that the players have some distance from the wall
	minPPos = minPPos - glm::vec2(OFFSET);
	maxPPos = maxPPos + glm::vec2(OFFSET);

	size = maxPPos - minPPos;
	glm::vec2 camMid = minPPos + 0.5f * size;

	// changes size to fit screen ratio
	if (abs(size.y) * WHratio > abs(size.x)) { // width is too small
		size.x = size.y * WHratio;
	}
	else { // heigth is too small
		size.y = size.x / WHratio;
	}

	// makes sure cam has a maxiumum zoom
	if (abs(size).y < minSizeHeight) {
		size *= minSizeHeight / abs(size).y;
	}

	pos = camMid - 0.5f * size;
}

