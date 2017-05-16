#pragma once

#include "EnergyBullet.h"

class EnergyBulletBig : public EnergyBullet
{
public:
	EnergyBulletBig(glm::vec2 position, GLfloat newAngle);
	~EnergyBulletBig();

	GLboolean UpdateE(GLfloat dt);

	GLboolean CheckForErase(glm::vec2 levelSize);
};

