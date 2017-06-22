#pragma once

#include "CompE.h"

class LivingE : public CompE
{
public:
	LivingE(glm::vec2 position);
	~LivingE();

	GLboolean UpdateE(GLfloat dt);
	void GetAttacked(GLfloat damage);

	// Getters and setters
	GLboolean GetDeath() const;
	GLfloat GetInherentF() const;

protected:
	GLboolean PreUpdate();

	GLfloat inherentF;
	GLfloat maxHealth;
	GLfloat health;
	GLboolean death;
	GLfloat inherentForce;
};

