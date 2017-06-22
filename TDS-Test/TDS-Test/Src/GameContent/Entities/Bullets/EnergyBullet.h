#pragma once

#include "..\..\..\Tools\Util.h"
#include "Bullet.h"

class EnergyBullet : public Bullet
{
public:
	static void loadEnergyBullet();
	
	EnergyBullet(glm::vec2 position, GLfloat angle, std::vector<const Entity*> damageWhitelist);
	EnergyBullet(glm::vec2 position, GLfloat angle, std::vector<const Entity*> damageWhitelist, std::vector<const Entity*> collisionWhitelist);
	virtual ~EnergyBullet();

	GLboolean CheckForErase(glm::vec2 levelSize);

protected:
	
};

