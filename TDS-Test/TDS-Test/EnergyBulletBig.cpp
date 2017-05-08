#include "EnergyBulletBig.h"



EnergyBulletBig::EnergyBulletBig(glm::vec2 position, GLfloat newAngle): EnergyBullet(position, newAngle)
{
	velocity = 15;
	vel = Util::create2DrotMatrix(glm::degrees(newAngle)) * glm::vec2(0, 1) * velocity;
	mass = 200000;
	size = glm::vec2(1.7, 3.5);
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