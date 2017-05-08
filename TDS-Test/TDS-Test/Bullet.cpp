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

void Bullet::Collision(Entity* cE, GLfloat dt) {
	collision = GL_TRUE;
	Enemy* myEnemy = dynamic_cast<Enemy*> (cE);
	if (myEnemy != NULL) {
		myEnemy->health -= damage;
	}
}
