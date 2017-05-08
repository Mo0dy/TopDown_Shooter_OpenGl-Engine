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
	collision = GL_TRUE;
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
		dynamicCollision(E2, dt);
	}
}