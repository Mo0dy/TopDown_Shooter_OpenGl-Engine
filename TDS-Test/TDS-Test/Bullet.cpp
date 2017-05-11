#include "Bullet.h"

Bullet::Bullet(glm::vec2 position, GLfloat newAngle) : DynE(position), velocity(200)
{
	angle = newAngle;
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
	enemy->health -= damage;
}

void Bullet::ColWithStat(Entity* cE, GLfloat colDepth) {
	collision = GL_TRUE;
}