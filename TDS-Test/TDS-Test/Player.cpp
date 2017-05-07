#include "Player.h"

Player::Player(glm::vec2 position) : DynE(position) {
}

Player::~Player()
{
	for (Entity *e : addEntities) {
		delete e;
	}
}

void Player::shoot() {
}

GLfloat Player::getInherentF() {
	return inherentForce;
}

std::vector<Entity*> Player::getAddEntities() { return addEntities; }

void Player::setColor(glm::vec3 color) {
	this->color = color;
	for (Entity *e : addEntities) {
		e->setColor(color);
	}
}