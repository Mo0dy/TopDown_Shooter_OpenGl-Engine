#include "Enemy.h"

Enemy::Enemy(glm::vec2 position) : DynE(position), lastAttack(9999)
{
}

Enemy::~Enemy()
{
}

GLboolean Enemy::checkForErase(glm::vec2 levelSize) {
	if (health < 0) { //|| pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}

void Enemy::ColWithPlayer(Player* player, GLfloat colDepth) {
	ColWithDyn(player, colDepth);
	if (attacking) {
		player->health -= damage;
	}
	attacking = GL_FALSE;
	lastAttack = 0;
}