#pragma once

#include "DynE.h"
//#include "Bullet.h"

class Enemy: public DynE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();
	
	GLfloat health;

	GLboolean checkForErase(glm::vec2 levelSize);

	//std::vector<Bullet*> damageWhitelist;

	GLfloat lastAttack;
	GLfloat attackSpeed;
	GLboolean attacking;
	GLfloat damage;
};

