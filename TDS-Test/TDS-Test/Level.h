#pragma once

#include "Util.h"

#include "Background.h"
#include "Entity.h"
#include "ResourceManager.h"

class Level
{
public:
	Level();
	~Level();
	glm::vec2 size;
	Background* background;
	std::vector<Entity*> entities;
};

