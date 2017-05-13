#include "EnergyBulletBig.h"
#include "Game.h"

GLboolean EnergyBulletBig::updateE(GLfloat dt) {
	//for (Enemy *e : Game::Enemies) {
	//	if (glm::distance(e->pos, pos) < 3) {
	//		e->addForce(glm::normalize(e->pos - pos) * 1000.0f);
	//	}
	//}
	pos += vel * dt;
	return GL_TRUE;
}

EnergyBulletBig::EnergyBulletBig(glm::vec2 position, GLfloat newAngle): EnergyBullet(position, newAngle)
{
	velocity = 15;
	vel = Util::rotationMat2(newAngle) * glm::vec2(0, 1) * velocity;
	mass = 200000;
	etex.setTexSize(glm::vec2(1.7, 3.5));
	damage = 90;

	color = glm::vec3(1.0f, 0.0f, 0.0f);
}

EnergyBulletBig::~EnergyBulletBig()
{
}

GLboolean EnergyBulletBig::checkForErase(glm::vec2 levelSize) {
	if (pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}