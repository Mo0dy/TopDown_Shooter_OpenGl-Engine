#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

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
