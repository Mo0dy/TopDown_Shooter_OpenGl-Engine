#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(glm::vec2 position) : LivingE(position), lastAttack(9999), attacking(GL_TRUE), death(GL_FALSE)
{
}

Enemy::~Enemy()
{
}

//Checks if this is dead or out of bounds
GLboolean Enemy::checkForErase(glm::vec2 levelSize) {
	if (death || pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}

void Enemy::ColWithPlayer(Player* player, GLfloat colDepth, glm::vec2 colAxis) {
	ColWithDyn(player, colDepth, colAxis);
	if (attacking) {
		player->health -= damage;
	}
	attacking = GL_FALSE;
	lastAttack = 0;
}

void Enemy::setBodyAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = calcMovAngle(angle, vel);
	if (abs(dA) > 0) {
		if (turnSpeed * dt > abs(dA)) {
			angle += dA;
		}
		else {
			angle += dA / abs(dA) * turnSpeed * dt;
		}
	}
}