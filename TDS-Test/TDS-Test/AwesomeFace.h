#pragma once

#include "Enemy.h"

class AwesomeFace: public Enemy
{
public:

	static void LoadAwesomeface();

	AwesomeFace(glm::vec2 position);
	~AwesomeFace();

	GLboolean updateE(GLfloat dt);
	GLfloat movForce;
};