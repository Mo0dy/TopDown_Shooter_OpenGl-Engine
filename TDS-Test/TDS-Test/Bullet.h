#pragma once

#include "Util.h"
#include "DynE.h"
#include "Enemy.h"

class Bullet : public DynE
{
public:
	Bullet(glm::vec2 position, GLfloat newAngle);
	~Bullet();

	GLboolean updateE(GLfloat dt);

	virtual void ColWithEnemy(class Enemy* enemy);
	void ColWithStat(Entity* cE, GLfloat colDepth);

	std::vector<Entity*> whitelist;
	GLfloat damage;

protected:
	GLfloat velocity;
};

