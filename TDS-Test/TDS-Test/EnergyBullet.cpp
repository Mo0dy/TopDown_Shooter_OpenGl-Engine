#include "EnergyBullet.h"
#include "ResourceManager.h"

void EnergyBullet::loadEnergyBullet() {
	ResourceManager::LoadEtex("Textures","EnergyBullet", ".png", GL_TRUE, "EnergyBall", GL_FALSE);
}

EnergyBullet::EnergyBullet(glm::vec2 position, GLfloat newAngle): Bullet(position, newAngle)
{
	velocity = 20;
	vel = Util::rotationMat2(newAngle) * glm::vec2(0, 1) * velocity;
	mass = 10;
	etex = ResourceManager::GetEtex("EnergyBall");
	etex.fitHitboxToTex();
	etex.setTexSize(glm::vec2(0.25, 0.6));
	updateHitboxes();
	angle = newAngle;
	damage = 10;
}

EnergyBullet::~EnergyBullet()
{
}

GLboolean EnergyBullet::checkForErase(glm::vec2 levelSize) {
	if (collision || pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}