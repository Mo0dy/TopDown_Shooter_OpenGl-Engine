#pragma once

#include "EnergyBullet.h"

class EnergyBulletBig : public EnergyBullet
{
public:
	EnergyBulletBig(glm::vec2 position, GLfloat newAngle);
	~EnergyBulletBig();

	GLboolean checkForErase(glm::vec2 levelSize);
};

