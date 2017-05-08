#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

Enemy::Enemy(glm::vec2 position) : DynE(position), lastAttack(9999)
{
}

Enemy::~Enemy()
{
}

GLboolean Enemy::checkForErase(glm::vec2 levelSize) {
	if (health < 0 || pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}

void Enemy::Collision(Entity* cE, GLfloat dt) {
	collision = GL_TRUE;
	Bullet* b = dynamic_cast<Bullet*> (cE);
	if (b != NULL) { // Collision with a Bullet
		GLboolean removeHealth = true;
		for (Bullet *wB : damageWhitelist) {
			if (wB == b) {
				removeHealth = false;
				break;
			}
		}
		if (removeHealth) {
			health -= b->damage;

			// Adding bullet to bullet whitelist so that no more damage is recieved
			damageWhitelist.push_back(b);
			// Keepas damage whitelist from becoming too full
			if (damageWhitelist.size() > 10) {
				damageWhitelist.erase(damageWhitelist.begin());
			}
		}
	}
	DynE* E2 = dynamic_cast<DynE*> (cE);
	if (E2 == NULL) { // Collision with a static object

	}
	else {
		Player* p = dynamic_cast<Player*> (E2);
		if (p != NULL && attacking) {
			attacking = GL_FALSE;
			lastAttack = 0;
		}
		dynamicCollision(E2, dt);
	}
}

