#pragma once

#include "DynE.h"
#include "Bullet.h"

class Enemy: public DynE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();
	
	GLfloat health;

	GLboolean checkForErase(glm::vec2 levelSize);

	void Collision(Entity* cE, GLfloat dt);

	std::vector<Bullet*> damageWhitelist;
};

