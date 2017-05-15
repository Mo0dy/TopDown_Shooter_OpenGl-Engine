#include "Player.h"

Player::Player(glm::vec2 position) : LivingE(position) {
}

Player::~Player()
{
}

GLfloat Player::getInherentF() {
	return inherentForce;
}