#include "EnergyBullet.h"
#include "ResourceManager.h"

void EnergyBullet::loadEnergyBullet() {
	ResourceManager::LoadTexture("Textures\\EnergyBullet.png", GL_TRUE, "EnergyBall");
}

EnergyBullet::EnergyBullet(glm::vec2 position, GLfloat newAngle): Bullet(position, newAngle)
{
	velocity = 20;
	vel = Util::create2DrotMatrix(glm::degrees(newAngle)) * glm::vec2(0, 1) * velocity;
	mass = 1;
	size = glm::vec2(0.25, 0.6);
	tex = "EnergyBall";
	damage = 10;
}

EnergyBullet::~EnergyBullet()
{
}

