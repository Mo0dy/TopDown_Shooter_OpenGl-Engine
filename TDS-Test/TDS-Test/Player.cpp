#include "Player.h"

Player::Player(glm::vec2 position) : CompE(position) {
}

Player::~Player()
{
}

GLfloat Player::getInherentF() {
	return inherentForce;
}