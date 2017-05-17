#include "EnergyBullet.h"
#include "ResourceManager.h"

void EnergyBullet::loadEnergyBullet() {
	ResourceManager::LoadEtex("Textures","EnergyBullet", ".png", GL_TRUE, "EnergyBall", GL_FALSE);
}

EnergyBullet::EnergyBullet(glm::vec2 position, GLfloat angle, std::vector<const LivingE*> whitelist) : Bullet(position, 20, angle, whitelist)
{
	mass = 10;
	tex = ResourceManager::GetEtex("EnergyBall").GetTex();
	hitObjs = ResourceManager::GetEtex("EnergyBall").GetHitObjs(glm::vec2(0.25, 0.6));
	damage = 10;
}

EnergyBullet::~EnergyBullet()
{
}

GLboolean EnergyBullet::CheckForErase(glm::vec2 levelSize) {
	if (collision || pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}