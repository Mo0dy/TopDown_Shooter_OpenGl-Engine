#include "EnergyBulletBig.h"
#include "Game.h"

GLboolean EnergyBulletBig::UpdateE(GLfloat dt) {
	//for (Enemy *e : Game::Enemies) {
	//	if (glm::distance(e->pos, pos) < 3) {
	//		e->addForce(glm::normalize(e->pos - pos) * 1000.0f);
	//	}
	//}
	pos += vel * dt;
	return GL_TRUE;
}


EnergyBulletBig::EnergyBulletBig(glm::vec2 position, GLfloat angle, std::vector<const Entity*> whitelist) : EnergyBullet(position, angle, whitelist)
{
	this->mass = 200000;
	this->tex = ResourceManager::GetEtex("EnergyBall").GetTex();
	this->hitObjs = ResourceManager::GetEtex("EnergyBall").GetHitObjs(glm::vec2(1.7, 3.5));
	this->size = glm::vec3(1.7, 3.5, 1.5f);
	this->damage = 90;
	this->vel = glm::normalize(this->vel) * 15.0f; // This should be done differently
}

EnergyBulletBig::~EnergyBulletBig()
{
}

GLboolean EnergyBulletBig::CheckForErase(glm::vec2 levelSize) {
	if (pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}