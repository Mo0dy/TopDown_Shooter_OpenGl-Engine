#pragma once

#include "Util.h"
#include "DynE.h"

class Bullet : public DynE
{
public:
	Bullet(glm::vec2 position, GLfloat newAngle);
	~Bullet();

	GLboolean updateE(GLfloat dt);

	std::vector<Entity*> whitelist;

protected:
	GLfloat velocity;
	GLfloat damage;
	void Collision(Entity* cE, GLfloat dt);
};

