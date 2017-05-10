#pragma once

#include "Enemy.h"

class E_Drone: public Enemy
{
public:

	static void load_E_Drone();

	E_Drone(glm::vec2 position);
	~E_Drone();

	GLboolean updateE(GLfloat dt);

	void setBodyAngle(GLfloat dt);
	GLfloat movForce;
	GLfloat turnSpeed;

	Animation* ani;

	GLfloat swarmFactor;
	GLfloat swarmPot;

	GLfloat maxHealth;
};