#pragma once

#include "CompE.h"

class LivingE : public CompE
{
public:
	LivingE(glm::vec2 position);
	~LivingE();

	virtual void GetAttacked(GLfloat damage);

	// Getters and setters
	GLboolean getDeath();

protected:
	GLfloat maxHealth;
	GLfloat health;
	GLboolean death;
};

