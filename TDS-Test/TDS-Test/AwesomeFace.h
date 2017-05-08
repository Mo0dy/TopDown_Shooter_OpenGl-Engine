#pragma once

#include "Enemy.h"

class AwesomeFace: public Enemy
{
public:

	static void loadAwesomeface();

	AwesomeFace(glm::vec2 position);
	~AwesomeFace();

	GLboolean updateE(GLfloat dt);
	GLfloat movForce;
};