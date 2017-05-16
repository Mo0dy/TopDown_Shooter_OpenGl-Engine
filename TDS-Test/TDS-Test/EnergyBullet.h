#pragma once

#include "Util.h"
#include "Bullet.h"

class EnergyBullet : public Bullet
{
public:
	static void loadEnergyBullet();

	EnergyBullet(glm::vec2 position, GLfloat newAngle);
	virtual ~EnergyBullet();

	GLboolean CheckForErase(glm::vec2 levelSize);

protected:
	
};

