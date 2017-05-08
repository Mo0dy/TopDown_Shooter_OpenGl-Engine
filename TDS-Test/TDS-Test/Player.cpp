#include "Player.h"
#include "Enemy.h"

Player::Player(glm::vec2 position) : CompE(position) {
}

Player::~Player()
{
}

GLfloat Player::getInherentF() {
	return inherentForce;
}

void Player::Collision(Entity* cE, GLfloat dt) {
	DynE* E2 = dynamic_cast<DynE*> (cE);
	if (E2 == NULL) { // Collision with static object

	}
	else {
		Enemy* myEnemy = dynamic_cast<Enemy*> (E2);
		if (myEnemy != NULL) {
			if (myEnemy->attacking) {
				health -= myEnemy->damage;
			}
		}
		collision = GL_TRUE;
		glm::vec2 c = E2->pos - pos;
		colVel = vel + 2 * ((E2->mass * glm::dot(E2->vel, c) - E2->mass * glm::dot(vel, c)) / glm::pow(glm::length(c), 2) / (mass + E2->mass)) * c;
		colPos = pos - c * COLLISION_ADD_CHANGE;

		// we should write that checks weather a collision is a true collision.
	}
}