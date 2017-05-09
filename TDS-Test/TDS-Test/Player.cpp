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
		dynCollision(E2);
		// we should write that checks weather a collision is a true collision.
	}
}