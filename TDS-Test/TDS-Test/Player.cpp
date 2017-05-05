#include "Player.h"

Player::Player(glm::vec2 position) : DynE(position) {
}

Player::~Player()
{
	for (Entity *e : addEntities) {
		delete e;
	}
}

GLfloat Player::getInherentF() {
	return inherentForce;
}

std::vector<Entity*> Player::getAddEntities() { return addEntities; }