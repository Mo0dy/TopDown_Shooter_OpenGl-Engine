#pragma once

#include "CompE.h"

class LivingE : public CompE
{
public:
	LivingE(glm::vec2 position);
	~LivingE();

	virtual void AddDamage(GLfloat damage);

	// Getters and setters
	GLboolean GetDeath();

protected:
	GLfloat maxHealth;
	GLfloat health;
	GLboolean death;
	GLfloat inherentForce;
	GLfloat turnSpeed;
};

