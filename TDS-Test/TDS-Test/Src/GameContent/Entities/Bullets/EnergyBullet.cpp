#include "EnergyBullet.h"
#include "..\..\..\OpenGl\ResourceManager.h"

void EnergyBullet::loadEnergyBullet() {
	ResourceManager::LoadEtex("Textures","EnergyBullet", ".png", GL_TRUE, "EnergyBall", HBOX_AUTOFIT);
}

EnergyBullet::EnergyBullet(glm::vec2 position, GLfloat angle, std::vector<const Entity*> damageWhitelist) : Bullet(position, angle, 20.0f, damageWhitelist)
{
	tex = ResourceManager::GetEtex("EnergyBall").GetTex();
	size = glm::vec3(0.25, 0.6, 0.25);
	hitComb = ResourceManager::GetEtex("EnergyBall").GetHitComb(glm::vec2(0.25, 0.6));
	damage = 10;
}

EnergyBullet::EnergyBullet(glm::vec2 position, GLfloat angle, std::vector<const Entity*> damageWhitelist, std::vector<const Entity*> collisionWhitelist) : EnergyBullet(position, angle, damageWhitelist)
{
	this->collisionWhitelist = collisionWhitelist;
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