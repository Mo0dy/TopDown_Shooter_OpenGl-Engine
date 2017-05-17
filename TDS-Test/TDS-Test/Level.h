#pragma once

#include "Util.h"

#include "Entity.h"
#include "ResourceManager.h"

class Level
{
public:
	Level();
	~Level();
	glm::vec2 size;
	Entity background;
	std::vector<Entity*> entities;

	virtual void UpdateL(GLfloat dt);
	virtual void Reset();
};

