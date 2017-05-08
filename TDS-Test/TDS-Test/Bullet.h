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
	GLfloat damage;

protected:
	GLfloat velocity;
	void Collision(Entity* cE, GLfloat dt);
};

