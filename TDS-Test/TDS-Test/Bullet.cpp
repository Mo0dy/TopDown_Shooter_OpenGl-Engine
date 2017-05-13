#include "Bullet.h"

Bullet::Bullet(glm::vec2 position, GLfloat newAngle) : DynE(position, angle), velocity(200)
{
	airFricCoeff = 0.0;
	state = MOVING;
}

Bullet::~Bullet()
{
}

GLboolean Bullet::updateE(GLfloat dt) {
	pos += vel * dt;
	return GL_TRUE;
}

void Bullet::ColWithEnemy(Enemy* enemy) {
	collision = GL_TRUE;
	if (!checkWhitelist(enemy)) {
		enemy->health -= damage;
		whitelist.push_back(enemy);
	}
}

void Bullet::ColWithStat(Entity* cE, GLfloat colDepth) {
	collision = GL_TRUE;
}

GLboolean Bullet::checkWhitelist(Enemy* enemy) {
	for (Entity *e : whitelist) {
		if (e == enemy) {
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}