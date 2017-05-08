#pragma once

#include "DynE.h"

class Enemy: public DynE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();
	
	GLfloat health;

	GLboolean checkForErase(glm::vec2 levelSize);
};

